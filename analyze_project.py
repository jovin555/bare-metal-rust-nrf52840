#!/usr/bin/env python3
"""Analyze the Zephyr project using DeepSeek AI."""

import os
import json
import urllib.request
import urllib.error

API_KEY = "sk-9a69706ab8ce4f1da53ed04589194329"
MODEL = "deepseek-chat"
BASE_URL = "https://api.deepseek.com/chat/completions"

PROJECT_CONTENT = """
=== PLAN (Docs/Plan.md) ===
Zephyr RTOS learning project using a structured multi-phase plan with Claude/AI.
The plan covers: Phase 1 (environment setup), Phase 2 (skeleton-first structural prompting),
Phase 3 (Plan-Execute-Isolate workflow), Phase 4 (4-milestone project roadmap:
Shell App → Multi-Threading → Hardware Overlays → Low-Power Bluetooth).

=== DAY 1 — Introduction to Zephyr RTOS (2026-05-16) ===
prj.conf: CONFIG_STDOUT_CONSOLE=y, CONFIG_PRINTK=y, CONFIG_MAIN_STACK_SIZE=1024
main.c:
  #include <zephyr/kernel.h>
  #include <zephyr/sys/printk.h>
  void main(void) {
      printk("Hello Zephyr! This is Day 1 of learning Zephyr.\\n");
      while (1) { k_msleep(1000); }
  }

=== DAY 2 — Zephyr Project Structure (2026-05-17) ===
prj.conf: CONFIG_STDOUT_CONSOLE=y, CONFIG_PRINTK=y, CONFIG_MAIN_STACK_SIZE=1024
main.c:
  void main(void) {
      printk("Day 2: Zephyr project structure example is running.\\n");
      while (1) { k_msleep(1000); }
  }

=== DAY 3 — Device Tree Basics (2026-05-18) ===
prj.conf: CONFIG_STDOUT_CONSOLE=y, CONFIG_PRINTK=y, CONFIG_MAIN_STACK_SIZE=1024
app.overlay: (custom device tree overlay file present)
main.c:
  #include <zephyr/device.h>
  #include <zephyr/devicetree.h>
  void main(void) {
      const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));
      if (!device_is_ready(uart)) {
          printk("Day 3: uart0 device is not ready.\\n");
      } else {
          printk("Day 3: uart0 device is ready via Device Tree.\\n");
      }
      while (1) { k_msleep(1000); }
  }

=== DAY 4 — Kconfig and prj.conf (2026-05-19) ===
prj.conf: CONFIG_LOG=y, CONFIG_LOG_DEFAULT_LEVEL=3
main.c:
  #include <zephyr/logging/log.h>
  LOG_MODULE_REGISTER(day19, LOG_LEVEL_INF);
  void main(void) {
      LOG_INF("Day 4: Kconfig and prj.conf example running.");
      while (1) { k_msleep(1000); }
  }

=== DAY 5 — Basic Zephyr Application (2026-05-20) ===
prj.conf: CONFIG_KERNEL_DEBUG=y
main.c:
  void main(void) {
      int count = 0;
      while (1) {
          printk("Day 5: Hello from Zephyr app loop (%d)!\\n", count++);
          k_msleep(K_SECONDS(1));
      }
  }

=== DAY 6 — Zephyr Drivers and Device Binding (2026-05-21) ===
prj.conf: CONFIG_UART_CONSOLE=y
main.c:
  #include <zephyr/drivers/uart.h>
  void main(void) {
      const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));
      if (!device_is_ready(uart)) { printk("Day 6: uart0 is not ready.\\n"); return; }
      printk("Day 6: uart0 is ready. Sending test output...\\n");
      uart_poll_out(uart, 'Z');
      uart_poll_out(uart, '\\n');
      while (1) { k_msleep(K_SECONDS(2)); }
  }

=== DAY 7 — Debugging and Testing (2026-05-22) ===
prj.conf: CONFIG_LOG=y, CONFIG_LOG_DEFAULT_LEVEL=3
main.c:
  LOG_MODULE_REGISTER(day22, LOG_LEVEL_INF);
  void main(void) {
      int step = 0;
      while (1) {
          LOG_INF("Day 7: debug step %d", step++);
          k_msleep(K_SECONDS(1));
      }
  }
"""

PROMPT = f"""You are an expert embedded systems engineer specializing in Zephyr RTOS.
Analyze the following Zephyr RTOS learning project that spans 7 days of progressive examples.

{PROJECT_CONTENT}

Please provide a comprehensive analysis covering:

1. **Learning Progression Assessment** — How well does the curriculum build from Day 1 to Day 7? What concepts are introduced effectively?

2. **Code Quality Review** — Review each day's code for correctness, best practices, and Zephyr API usage. Flag anything incorrect or deprecated.

3. **Issues and Bugs** — Identify any bugs, deprecated APIs, or potential runtime issues in the code examples.

4. **Missing Concepts** — What important Zephyr topics are missing from this 7-day curriculum that a beginner should learn?

5. **Suggested Next Steps** — Based on the plan's Phase 4 roadmap (Shell → Multi-Threading → Hardware Overlays → BLE), provide concrete recommendations for Days 8–14.

6. **Overall Rating** — Rate the project as a learning resource (1–10) and summarize your findings.

Be specific, technical, and actionable. Reference exact file/day names when citing issues."""


def call_deepseek(prompt: str) -> str:
    payload = {
        "model": MODEL,
        "messages": [
            {"role": "user", "content": prompt}
        ],
        "temperature": 0.7,
        "max_tokens": 3000,
    }

    data = json.dumps(payload).encode("utf-8")
    req = urllib.request.Request(
        BASE_URL,
        data=data,
        headers={
            "Content-Type": "application/json",
            "Authorization": f"Bearer {API_KEY}",
        },
        method="POST",
    )

    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            result = json.loads(resp.read().decode("utf-8"))
            return result["choices"][0]["message"]["content"]
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8")
        return f"HTTP Error {e.code}: {body}"
    except Exception as e:
        return f"Error: {e}"


if __name__ == "__main__":
    print("Connecting to DeepSeek AI...")
    print(f"Model: {MODEL}\n")
    print("=" * 60)

    analysis = call_deepseek(PROMPT)

    print(analysis)
    print("\n" + "=" * 60)

    # Save the analysis to a file
    output_path = os.path.join(os.path.dirname(__file__), "Docs", "AI_Analysis.md")
    with open(output_path, "w") as f:
        f.write("# DeepSeek AI Project Analysis\n\n")
        f.write(f"**Model:** {MODEL}  \n")
        f.write("**Date:** 2026-05-16  \n\n")
        f.write("---\n\n")
        f.write(analysis)

    print(f"\nAnalysis saved to: {output_path}")
