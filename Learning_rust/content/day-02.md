---
title: "Day 02: Functions, Return Values & Expressions"
date: 2026-06-24
tags: [til, rust, functions, expressions, return-values]
---

# Day 02: Functions, Return Values & Expressions

## What I Explored Today

Yesterday I learned that `fn main()` is the entry point of every Rust program. Today I went deeper into functions — how to define them, pass arguments, and return values. But the biggest "aha!" moment was understanding the difference between **statements** and **expressions**, which is the secret sauce behind Rust's elegant syntax.

## Writing Your First Function

Functions in Rust use `snake_case` by convention. Here's the anatomy:

```rust
// 'fn' keyword, function name, parameters with types, return type with ->
fn greet(name: &str) -> String {
    // The last expression is the return value (no semicolon!)
    format!("Hello, {}!", name)
}

fn main() {
    let message = greet("Alice");
    println!("{}", message); // Prints: Hello, Alice!
}
```

**Key observations:**
- Parameters **must** have type annotations — Rust won't guess them
- The return type comes after `->`
- Notice `format!` returns a `String`, and we didn't write `return`

## The Two Ways to Return Values

Rust gives you two options, but one is preferred:

```rust
// Explicit return (use for early returns)
fn is_even(x: i32) -> bool {
    if x % 2 == 0 {
        return true;  // Early return, needs 'return' keyword
    }
    false  // Implicit return — last expression, no semicolon
}

// Implicit return (idiomatic Rust)
fn double(x: i32) -> i32 {
    x * 2  // No semicolon = return this value
}

fn main() {
    println!("{}", is_even(4));   // true
    println!("{}", double(3));    // 6
}
```

**The golden rule:** If you put a semicolon, it becomes a statement (no value). If you omit it, it's an expression (has a value).

## Statements vs Expressions — The Core Concept

This is where Rust really clicked for me. Understanding this difference is crucial:

```rust
fn main() {
    // STATEMENT: performs an action, no value
    let x = 5;          // This is a statement
    println!("{}", x);  // This is a statement
    
    // EXPRESSION: evaluates to a value
    let y = {
        let a = 2;
        let b = 3;
        a + b  // This block is an expression! No semicolon!
    };
    // y = 5
    
    // EXPRESSIONS can be used anywhere values are expected
    let z = if y > 5 { 10 } else { 20 };
    // z = 20 because y (5) is NOT > 5
}
```

**The compiler's view:**
- `let x = 5;` → "Declare x and bind it to 5" (statement)
- `5` → "The value five" (expression)
- `a + b` → "Add a and b, producing a value" (expression)
- `let y = ...;` → "Assign the result of this block to y" (statement)

## Function Bodies Are Expressions Too!

Every function body is a block expression. This means you can do cool things like:

```rust
fn classify_number(x: i32) -> &'static str {
    // The entire function body is one big expression
    if x > 0 {
        "positive"
    } else if x < 0 {
        "negative"
    } else {
        "zero"
    }
    // No semicolon after any of these branches!
}

fn main() {
    println!("{}", classify_number(-5));  // negative
}
```

Notice how `if/else` works as an expression here. Each arm returns a `&str`, and the whole `if/else` evaluates to one of them.

## Compiler Fight: The Semicolon Trap

Here's a mistake I made at least three times today:

```rust
// WRONG — compiler error!
fn add(x: i32, y: i32) -> i32 {
    x + y;  // ERROR: expected i32, found ()
}
```

**The error message:**
```
error[E0308]: mismatched types
 --> src/main.rs:2:18
  |
2 | fn add(x: i32, y: i32) -> i32 {
  |    ---                     ^^^ expected `i32`, found `()`
  |    |
  |    implicitly returns `()` as its body has no tail or `return` expression
  |
help: consider removing this semicolon
  |
2 -     x + y;
2 +     x + y
```

The compiler literally tells you to remove the semicolon! The expression `x + y` evaluates to an `i32`, but adding `;` turns it into a statement that evaluates to `()` (unit type, Rust's version of "nothing").

## Multiple Return Values with Tuples

Need to return more than one thing? Use a tuple:

```rust
fn split_at_whitespace(text: &str) -> (&str, &str) {
    // Find first space, return before and after
    match text.find(' ') {
        Some(pos) => (&text[..pos], &text[pos+1..]),
        None => (text, ""),
    }
}

fn main() {
    let (first, last) = split_at_whitespace("hello world");
    println!("First: '{}', Last: '{}'", first, last);
    // Output: First: 'hello', Last: 'world'
}
```

## Practical Function Patterns

Here's a more realistic example combining everything:

```rust
/// Calculate BMI and return category
fn bmi_category(weight_kg: f64, height_m: f64) -> &'static str {
    let bmi = weight_kg / (height_m * height_m);
    
    // Using match as an expression
    match bmi {
        b if b < 18.5 => "underweight",
        b if b < 25.0 => "normal",
        b if b < 30.0 => "overweight",
        _ => "obese",
    }
}

fn main() {
    let category = bmi_category(70.0, 1.75);
    println!("BMI category: {}", category);  // normal
}
```

## Try It Yourself

Write a function `max_of_three` that takes three `i32` values and returns the largest. Use expressions (no explicit `return` statements except maybe for early exits).

```rust
// Your code here
fn max_of_three(a: i32, b: i32, c: i32) -> i32 {
    // Hint: nested if/else works as an expression
    // Or use std::cmp::max if you want to cheat ;)
    todo!()
}

fn main() {
    println!("{}", max_of_three(5, 12, 3));  // Should print 12
}
```

**Challenge:** Can you write it as a single expression without any `let` bindings inside the function body?

---
*Next up → **Day 03:** Control Flow: if, loop, while, for*