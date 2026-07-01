---
title: "Day 05: References & Borrowing"
date: 2026-06-24
tags: [til, rust, ownership, references, borrowing]
---

# Day 05: References & Borrowing

## What I Explored Today

Yesterday I learned that ownership moves—and that once a value is moved, the original owner can't use it anymore. That feels restrictive. What if I just want to *look* at a value without taking ownership? Today I learned Rust's elegant solution: **references** and **borrowing**.

The key insight: instead of transferring ownership, we can *borrow* a value temporarily. The borrower gets access, but the original owner stays the owner.

## Creating References with `&`

A reference is like a pointer that safely follows Rust's ownership rules. We create a reference with `&` and follow it (dereference) with `*`.

```rust
fn main() {
    let s = String::from("hello");
    
    // Create a reference to s — we're borrowing, not taking ownership
    let len = calculate_length(&s);
    
    // s is still usable here because we only lent it out
    println!("'{}' has length {}", s, len);
}

fn calculate_length(s: &String) -> usize {  // s is a reference to a String
    s.len()  // we can use s, but we don't own it
}  // s goes out of scope, but since it doesn't own the value, nothing is dropped
```

Notice `calculate_length` takes `&String` instead of `String`. The function borrows the value—it can read it, but it can't drop it or transfer ownership. When the function ends, the reference goes away, but the original `String` in `main` lives on.

## Mutable References: Borrowing with Write Access

What if we need to modify a borrowed value? Rust has a separate concept: **mutable references** with `&mut`.

```rust
fn main() {
    let mut s = String::from("hello");  // must be explicitly mutable
    
    change(&mut s);  // pass a mutable reference
    
    println!("{}", s);  // prints "hello, world!"
}

fn change(s: &mut String) {  // accept mutable reference
    s.push_str(", world!");  // can modify the borrowed value
}
```

This works, but with a crucial restriction: **you can only have one mutable reference to a particular piece of data at a time**.

```rust
let mut s = String::from("hello");

let r1 = &mut s;
let r2 = &mut s;  // ERROR! Cannot borrow `s` as mutable more than once

println!("{}, {}", r1, r2);
```

This restriction prevents **data races** at compile time. If two parts of code could modify the same data simultaneously, chaos ensues. Rust simply forbids it.

## The Golden Rule: Readers or One Writer

Rust enforces a simple rule: **at any given time, you can have either one mutable reference or any number of immutable references**.

```rust
let mut s = String::from("hello");

let r1 = &s;       // OK: multiple immutable references allowed
let r2 = &s;       // OK: r1 and r2 both borrow immutably
let r3 = &mut s;   // ERROR: can't have immutable AND mutable references

println!("{}, {}, {}", r1, r2, r3);
```

The compiler prevents the combination because an immutable reference assumes the value won't change—but a mutable reference could change it at any moment.

This restriction disappears once the immutable references are no longer used:

```rust
let mut s = String::from("hello");

let r1 = &s;  // no problem
let r2 = &s;  // no problem
println!("{} and {}", r1, r2);  // r1 and r2 are last used here

let r3 = &mut s;  // OK: no immutable references in scope anymore
println!("{}", r3);
```

The compiler tracks the **last use** of each reference (called the Non-Lexical Lifetimes, or NLL). Once `r1` and `r2` are done, `r3` can safely take a mutable reference.

## Dangling References: A Promise Rust Keeps

In languages with manual memory management, it's easy to create a **dangling pointer**—a reference to memory that's been freed. Rust prevents this at compile time.

```rust
fn dangle() -> &String {  // returns a reference to a String
    let s = String::from("hello");
    &s  // we return a reference to s
}  // But s goes out of scope here and is dropped!
   // The reference now points to invalid memory
```

The compiler catches this:

```
error[E0106]: missing lifetime specifier
 --> src/main.rs:1:16
  |
1 | fn dangle() -> &String {
  |                ^ expected named lifetime parameter
  |
  = help: this function's return type contains a borrowed value,
          but there is no value for it to be borrowed from
```

The fix: return the `String` directly, transferring ownership instead of borrowing:

```rust
fn no_dangle() -> String {
    let s = String::from("hello");
    s  // ownership moves out, nothing is dropped
}
```

## The Compiler Fight

Today's fight: trying to mutate through an immutable reference.

```rust
fn main() {
    let s = String::from("hello");
    let r = &s;
    r.push_str(", world!");  // ERROR!
}
```

```
error[E0596]: cannot borrow `*r` as mutable, as `r` is behind a `&` reference
 --> src/main.rs:4:5
  |
3 |     let r = &s;
  |              -- help: consider changing this to be a mutable reference: `&mut s`
4 |     r.push_str(", world!");
  |     ^^^^^^^^^^^^^^^^^^^^^^ `r` is a `&` reference, so the data it refers to cannot be borrowed as mutable
```

This makes sense: `&` means "I promise not to modify this." To mutate, we need `&mut`—and the original variable must be `mut` too.

## Try It Yourself

Write a function that takes a mutable reference to a `String` and adds " is learning Rust!" to it. Then call it from `main` with your name.

```rust
fn main() {
    let mut name = String::from("Alice");
    add_rust_enthusiasm(&mut name);
    println!("{}", name);
}

fn add_rust_enthusiasm(s: &mut String) {
    // Your code here
}
```

**Challenge**: What happens if you try to call `add_rust_enthusiasm` twice with the same `&mut name` without using the result of the first call? Try it and see the compiler error.

---
*Next up → **Day 06:** The Slice Type*