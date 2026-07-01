---
title: "Day 04: Ownership — The Core Mental Model"
date: 2026-06-24
tags: [til, rust, ownership, memory]
---

# Day 04: Ownership — The Core Mental Model

## What I Explored Today

Yesterday I felt like I was writing Rust with training wheels — using `.clone()` everywhere because I didn't understand the rules. Today I removed the wheels. I learned **ownership**, Rust's most unique feature and the reason it can guarantee memory safety without a garbage collector.

Ownership is Rust's answer to three classic memory management problems:
1. **Dangling pointers** — using memory after it's freed
2. **Double free** — freeing the same memory twice
3. **Memory leaks** — never freeing memory you're done with

Instead of a garbage collector or manual `malloc`/`free`, Rust uses a compiler-enforced set of rules. Once they click, you'll never want to go back.

## The Three Rules of Ownership

Every Rust programmer has these rules memorized:

1. **Each value in Rust has exactly one owner.**
2. **There can only be one owner at a time.**
3. **When the owner goes out of scope, the value is dropped.**

Let's see what that looks like in practice.

## Moving: Transferring Ownership

The most common ownership operation is a **move**. When you assign one variable to another (for types that don't implement `Copy`), ownership transfers.

```rust
fn main() {
    let s1 = String::from("hello"); // s1 is the owner of the string "hello"
    let s2 = s1;                    // Ownership MOVES from s1 to s2
    
    // println!("{}", s1); // COMPILE ERROR! s1 no longer owns the value
    println!("{}", s2);    // ✅ Works fine — s2 is now the owner
}
```

**Why does this happen?** Behind the scenes, Rust doesn't make a deep copy of the string data. Instead, it copies the pointer, length, and capacity (the "stack part" of the `String`), then *invalidates* the original. This is fast — no heap allocation needed.

## Clone: Explicit Deep Copy

If you *do* want a full copy, use `.clone()`:

```rust
fn main() {
    let s1 = String::from("hello");
    let s2 = s1.clone(); // Deep copy: both s1 and s2 are valid
    
    println!("s1 = {}, s2 = {}", s1, s2); // ✅ Both work!
}
```

Think of `.clone()` as saying "I know this costs memory and time, and I'm okay with that." It's the explicit version of what other languages do silently.

## Copy Types: The Exception

Some types are stored entirely on the stack and are cheap to copy. These implement the `Copy` trait and are *always* copied, never moved:

```rust
fn main() {
    let x = 5;      // i32 implements Copy
    let y = x;      // x is COPIED, not moved
    
    println!("x = {}, y = {}", x, y); // ✅ Both still valid!
}
```

Common `Copy` types:
- All integers (`i32`, `u64`, etc.)
- Floating-point numbers (`f64`, `f32`)
- Booleans (`bool`)
- Characters (`char`)
- Tuples of only `Copy` types (e.g., `(i32, i32)`)

**Key insight:** `String` is *not* `Copy` because it owns heap memory. An `i32` is always `Copy` because it's just a few bytes on the stack.

## Ownership and Functions

Ownership rules apply when passing values to functions too. Passing a value *moves* it into the function:

```rust
fn take_ownership(s: String) {
    println!("Inside function: {}", s);
    // s goes out of scope here and is dropped (memory freed)
}

fn main() {
    let my_string = String::from("data");
    take_ownership(my_string);  // Ownership moves INTO the function
    
    // println!("{}", my_string); // COMPILE ERROR! Ownership was moved
}
```

To get ownership back, you have to return it:

```rust
fn give_and_take(s: String) -> String {
    println!("Got: {}", s);
    s  // Return ownership back to caller
}

fn main() {
    let s1 = String::from("ping pong");
    let s2 = give_and_take(s1);  // s1 moves in, s2 gets ownership back
    
    // println!("{}", s1); // Still an error — s1 was moved
    println!("{}", s2);    // ✅ s2 owns it now
}
```

This "pass it in, get it back" pattern is tedious. Tomorrow's topic — references — will fix this.

## Compiler Fight: The Move After Clone Mistake

Here's a subtle error I made today:

```rust
fn main() {
    let s = String::from("hello");
    let t = s.clone();
    println!("{}", s); // ✅ Works — s wasn't moved
    
    let u = s;         // Now s MOVES to u
    println!("{}", t); // ✅ Works — t has its own copy
    println!("{}", s); // ❌ ERROR: borrow of moved value: `s`
}
```

**The fix:** Once you move ownership, stop using the old variable. Or don't move it — use a reference instead (stay tuned for Day 05).

## Scope and Drop

Rust automatically frees memory when the owner goes out of scope:

```rust
fn main() {
    {                              // Inner scope begins
        let inner = String::from("temporary");
        println!("{}", inner);     // ✅ Accessible here
    }                              // inner goes out of scope → memory freed
    
    // println!("{}", inner);      // COMPILE ERROR: not in scope
}
```

This is Rust's version of a destructor. The compiler inserts a call to `drop()` at the closing brace. You never see it, but it's there — guaranteed.

## Why This Matters

Ownership eliminates entire categories of bugs at compile time:
- **No dangling pointers**: the owner always exists while the value is accessible
- **No double free**: only the owner can free the memory, and it does so exactly once
- **No garbage collector pauses**: memory is freed deterministically when scope ends

The trade-off is that you have to think about ownership *while writing code*, not after. At first it feels restrictive. By Day 10, it'll feel natural.

## Try It Yourself

```rust
// TODO: Fix the ownership errors below
fn main() {
    let data = String::from("Rust ownership");
    let copied = data.clone();  // ✅ Deep copy
    
    // ❌ This line has an ownership bug — fix it!
    let moved = data;  // HINT: Is this a move or a copy?
    
    println!("copied: {}", copied);
    println!("moved: {}", moved);
    // println!("data: {}", data); // This line causes an error — why?
}
```

**Challenge:** Change the code so that `data` is accessible at the end. You have two options — which one is more efficient?

---
*Next up → **Day 05:** References & Borrowing — How to use values without taking ownership*