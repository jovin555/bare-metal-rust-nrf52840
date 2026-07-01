---
title: "Day 10: Option<T> — Handling the Absence of a Value"
date: 2026-06-24
tags: [til, rust, enums, option, null-safety]
---

# Day 10: Option<T> — Handling the Absence of a Value

## What I Explored Today

Every programmer has faced the dreaded null pointer exception. In many languages, `null` is a value that can be assigned to any variable — and then crash your program when you try to use it. Rust takes a different approach: it completely eliminates null. Instead, Rust uses an enum called `Option<T>` to represent the *possibility* of a value being absent.

Today I learned how `Option<T>` works, why it's safer than null, and how to work with it using pattern matching and helper methods.

## The Problem That Null Creates

In languages like Java, C#, or JavaScript, you can write code that assumes a value exists when it doesn't:

```rust
// This is NOT valid Rust — it's a thought experiment
// In other languages, this would compile and crash at runtime:
// let name = null;
// println!("{}", name.to_uppercase()); // BOOM! NullPointerException!
```

Rust says: "If a value might be missing, make that explicit in the type system." Enter `Option<T>`.

## What Is Option<T>?

`Option<T>` is an enum defined in Rust's standard library:

```rust
enum Option<T> {
    Some(T),    // A value of type T exists
    None,       // No value exists
}
```

The `<T>` means it's generic — it works with any type. `Option<i32>` can hold an integer or nothing. `Option<String>` can hold a string or nothing.

## Creating and Using Option Values

Let's see how to create `Option` values:

```rust
fn main() {
    // Create an Option containing a value
    let some_number: Option<i32> = Some(42);
    let some_string: Option<&str> = Some("hello");
    
    // Create an Option representing "no value"
    let no_number: Option<i32> = None;
    
    // Rust can often infer the type from context
    let inferred_some = Some(3.14);         // Option<f64>
    let inferred_none: Option<bool> = None; // Must specify type for None
    
    println!("We have: {:?} and {:?}", some_number, no_number);
}
```

The key insight: `Option<i32>` and `i32` are **different types**. Rust won't let you use them interchangeably:

```rust
fn main() {
    let x: i32 = 5;
    let y: Option<i32> = Some(10);
    
    // This won't compile:
    // let sum = x + y; // ERROR: cannot add `i32` to `Option<i32>`
    
    // You must extract the value first!
    match y {
        Some(value) => {
            let sum = x + value;
            println!("Sum is: {}", sum); // Works!
        }
        None => {
            println!("y has no value, can't add");
        }
    }
}
```

## The Compiler Fight: Forgetting to Handle None

Here's a classic mistake and how the compiler catches it:

```rust
// WRONG — compiler will reject this
fn get_middle_name(full_name: &str) -> Option<&str> {
    let parts: Vec<&str> = full_name.split_whitespace().collect();
    if parts.len() >= 3 {
        Some(parts[1]) // Return the middle name
    } else {
        None // No middle name exists
    }
}

fn main() {
    let name = get_middle_name("John Smith");
    
    // COMPILER ERROR: this tries to use Option<&str> as &str
    // println!("Middle name: {}", name);
    
    // FIX: Handle both cases
    match name {
        Some(middle) => println!("Middle name: {}", middle),
        None => println!("No middle name found"),
    }
}
```

The compiler forces you to acknowledge that `None` is a possibility. This is the entire point — no more surprise crashes!

## Pattern Matching with Option

The most explicit way to work with `Option` is `match`:

```rust
fn divide(numerator: f64, denominator: f64) -> Option<f64> {
    if denominator == 0.0 {
        None // Can't divide by zero
    } else {
        Some(numerator / denominator)
    }
}

fn main() {
    let result = divide(10.0, 2.0);
    
    match result {
        Some(value) => println!("Result: {}", value),
        None => println!("Cannot divide by zero!"),
    }
    
    // You can also use if let for simple cases
    let another = divide(5.0, 0.0);
    if let Some(val) = another {
        println!("Got: {}", val);
    } else {
        println!("Division failed");
    }
}
```

## Useful Option Methods

Rust provides many helper methods on `Option` to avoid writing match statements for common patterns:

```rust
fn main() {
    let a: Option<i32> = Some(42);
    let b: Option<i32> = None;
    
    // unwrap_or: provide a default value
    println!("a = {}", a.unwrap_or(0));  // prints 42
    println!("b = {}", b.unwrap_or(0));  // prints 0
    
    // map: transform the inner value if it exists
    let doubled = a.map(|x| x * 2);
    println!("Doubled: {:?}", doubled); // Some(84)
    
    let nothing_doubled = b.map(|x| x * 2);
    println!("Nothing doubled: {:?}", nothing_doubled); // None
    
    // is_some / is_none: simple checks
    println!("a has value: {}", a.is_some());  // true
    println!("b is none: {}", b.is_none());    // true
    
    // and_then: chain operations that return Option
    let config_file: Option<&str> = Some("config.txt");
    let contents = config_file.and_then(read_file);
    // If config_file is None, read_file is never called!
}

fn read_file(path: &str) -> Option<String> {
    // Pretend this reads a file
    if path == "config.txt" {
        Some("server=localhost".to_string())
    } else {
        None
    }
}
```

## A Real-World Example: Finding a User

Here's how `Option` makes real code safer:

```rust
struct User {
    id: u32,
    name: String,
}

fn find_user_by_id(id: u32) -> Option<User> {
    // Pretend database lookup
    let users = vec![
        User { id: 1, name: "Alice".to_string() },
        User { id: 2, name: "Bob".to_string() },
    ];
    
    users.into_iter().find(|u| u.id == id)
}

fn main() {
    let user = find_user_by_id(3); // This user doesn't exist
    
    // In other languages, this might return null and crash later
    // In Rust, we MUST handle the None case
    
    match user {
        Some(u) => println!("Found user: {}", u.name),
        None => println!("User not found"), // No crash!
    }
    
    // Or use the ? operator (we'll learn more later)
    // let name = find_user_by_id(1)?.name; // Only if we're in a function returning Option
}
```

## Try It Yourself

Write a function that takes an `Option<f64>` and returns the square root if the value exists and is non-negative, or `None` otherwise. Then write a `main` function that tests it with `Some(9.0)`, `Some(-4.0)`, and `None`.

```rust
fn safe_sqrt(value: Option<f64>) -> Option<f64> {
    // Your code here
    // Hint: check if value is Some and non-negative
    todo!()
}

fn main() {
    // Test your function
    println!("{:?}", safe_sqrt(Some(9.0)));   // Should be Some(3.0)
    println!("{:?}", safe_sqrt(Some(-4.0)));  // Should be None
    println!("{:?}", safe_sqrt(None));         // Should be None
}
```

---

*Next up → **Day 11:** Packages, Crates & Modules — Organizing Your Rust Code*