#!/usr/bin/env python3
"""
Daily Rust blog post generator.
Calls DeepSeek to write the next day's TIL entry, then pushes to GitHub.
"""

import os
import re
import sys
import json
import datetime
import subprocess
import requests
from pathlib import Path

# ── Paths ──────────────────────────────────────────────────────────────────
SCRIPT_DIR   = Path(__file__).parent
ENV_FILE     = SCRIPT_DIR / ".env"
BLOG_DIR     = Path("/home/eva/workspace/My_Bare_metal_nrf52840/Learning_rust")
CONTENT_DIR  = BLOG_DIR / "content"
INDEX_FILE   = CONTENT_DIR / "index.md"

# ── Load .env ──────────────────────────────────────────────────────────────
def load_env(path: Path) -> dict:
    env = {}
    for line in path.read_text().splitlines():
        line = line.strip()
        if line and not line.startswith("#") and "=" in line:
            k, _, v = line.partition("=")
            env[k.strip()] = v.strip().strip('"')
    return env

ENV = load_env(ENV_FILE)
API_KEY = ENV["LLM_API_KEY"]
MODEL   = ENV.get("LLM_MODEL", "deepseek-chat")

# ── Rust curriculum (ordered) ──────────────────────────────────────────────
CURRICULUM = [
    "Variables, Mutability & Basic Types",                    # Day 01 done
    "Functions, Return Values & Expressions",
    "Control Flow: if, loop, while, for",
    "Ownership: The Core Mental Model",
    "References & Borrowing",
    "The Slice Type",
    "Structs: Defining & Instantiating",
    "Methods on Structs",
    "Enums & Pattern Matching with match",
    "Option<T> — Handling the Absence of a Value",
    "Packages, Crates & Modules",
    "Vectors: Growing Lists",
    "Strings: &str vs String in Depth",
    "HashMaps",
    "Error Handling with Result<T, E>",
    "Generics: Writing Code for Any Type",
    "Traits: Shared Behaviour",
    "Lifetimes: Preventing Dangling References",
    "Closures: Anonymous Functions",
    "Iterators & the Iterator Trait",
    "Box<T> and the Heap",
    "Rc<T> and Multiple Ownership",
    "RefCell<T> and Interior Mutability",
    "Threads & Concurrency Basics",
    "Channels: Message Passing Between Threads",
    "Mutex<T> and Shared State Concurrency",
    "Async / Await Basics",
    "Writing & Running Tests",
    "Building a CLI Tool End-to-End",
    "Capstone: Mini Project Wrap-up",
]

# ── Determine next day ─────────────────────────────────────────────────────
def next_day() -> tuple[int, str]:
    """Return (day_number, topic) for the next post to generate."""
    existing = sorted(CONTENT_DIR.glob("day-*.md"))
    day_num  = len(existing) + 1          # day-01 already exists → starts at 2
    if day_num - 1 >= len(CURRICULUM):
        print("All curriculum topics have been published!")
        sys.exit(0)
    return day_num, CURRICULUM[day_num - 1]

# ── Call DeepSeek ──────────────────────────────────────────────────────────
SYSTEM_PROMPT = """\
You are an expert Rust teacher writing daily TIL (Today I Learned) blog posts for a beginner \
learning Rust from scratch. Your posts are:
- Structured, clear, and beginner-friendly
- Heavy on annotated code examples with comments explaining WHY
- Include at least one "compiler fight" section showing a common error + fix where relevant
- End with a short "Try It Yourself" snippet and a one-line teaser for the next day
- Use GitHub-Flavored Markdown with fenced code blocks tagged `rust`
- Tone: encouraging, precise, never condescending

Format every post exactly like this:
---
title: "Day NN: <topic>"
date: YYYY-MM-DD
tags: [til, <relevant-tags>]
---

# Day NN: <topic>

## What I Explored Today
...

## <Section 1 heading>
...

## <Section 2 heading>
...

## Try It Yourself
...

---
*Next up → **Day NN+1:** <next topic>*
"""

def generate_post(day_num: int, topic: str, next_topic: str) -> str:
    today = datetime.date.today().isoformat()
    user_msg = (
        f"Write Day {day_num:02d} of my Rust learning blog.\n"
        f"Topic: {topic}\n"
        f"Date: {today}\n"
        f"Next day's topic: {next_topic}\n\n"
        "Make it thorough — at least 600 words of content with multiple code examples."
    )
    resp = requests.post(
        "https://api.deepseek.com/chat/completions",
        headers={"Authorization": f"Bearer {API_KEY}", "Content-Type": "application/json"},
        json={
            "model": MODEL,
            "messages": [
                {"role": "system", "content": SYSTEM_PROMPT},
                {"role": "user",   "content": user_msg},
            ],
            "temperature": 0.7,
            "max_tokens": 4096,
        },
        timeout=120,
    )
    resp.raise_for_status()
    return resp.json()["choices"][0]["message"]["content"]

# ── Update index.md ────────────────────────────────────────────────────────
def update_index(day_num: int, topic: str, tags: list[str]):
    text    = INDEX_FILE.read_text()
    tag_str = " ".join(f"`#{t}`" for t in tags)
    new_row = f"| [[day-{day_num:02d}\\|Day {day_num:02d}]] | {topic} | {tag_str} |"
    # Find the last table row and append after it
    last_row_match = list(re.finditer(r"^\| \[\[day-\d+.*?\|$", text, re.MULTILINE))
    if last_row_match:
        insert_pos = last_row_match[-1].end()
        text = text[:insert_pos] + "\n" + new_row + text[insert_pos:]
    else:
        # Fallback: insert before the closing ---
        text = text.replace("\n---\n", f"\n{new_row}\n\n---\n", 1)
    INDEX_FILE.write_text(text)

# ── Git push ───────────────────────────────────────────────────────────────
def git_push(day_num: int, topic: str):
    def run(cmd):
        result = subprocess.run(cmd, cwd=BLOG_DIR, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"[git error] {result.stderr.strip()}")
            sys.exit(1)
        return result.stdout.strip()

    run(["git", "add", "content/"])
    run(["git", "commit", "-m", f"Day {day_num:02d}: {topic}"])
    run(["git", "push", "origin", "main"])
    print(f"  Pushed Day {day_num:02d} to GitHub.")

# ── Main ───────────────────────────────────────────────────────────────────
def main():
    day_num, topic = next_day()
    next_idx       = day_num          # curriculum is 0-indexed; day_num is 1-indexed
    next_topic     = CURRICULUM[next_idx] if next_idx < len(CURRICULUM) else "Capstone Complete"

    print(f"[{datetime.date.today()}] Generating Day {day_num:02d}: {topic}")

    post = generate_post(day_num, topic, next_topic)

    # Extract tags from frontmatter if present, else default
    tag_match = re.search(r"^tags:\s*\[(.+?)\]", post, re.MULTILINE)
    tags = [t.strip() for t in tag_match.group(1).split(",")] if tag_match else ["til"]

    out_file = CONTENT_DIR / f"day-{day_num:02d}.md"
    out_file.write_text(post)
    print(f"  Written: {out_file.name}")

    update_index(day_num, topic, tags)
    print(f"  Updated: index.md")

    print(f"  Done. Day {day_num:02d} written to {out_file}.")

if __name__ == "__main__":
    main()
