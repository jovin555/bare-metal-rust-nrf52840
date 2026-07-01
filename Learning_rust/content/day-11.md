---
title: "Day 11: Packages, Crates & Modules"
date: 2026-06-24
tags: [til, rust, modules, project-structure]
---

# Day 11: Packages, Crates & Modules

## What I Explored Today

Yesterday I learned about structs and enums. Today I'm zooming way out to understand how Rust projects are organized. If you've ever opened a Python project and seen `import` everywhere, or a JavaScript project with `require`/`import` statements, you already know the *why*: we need to split code into files and control what's visible where.

Rust's module system is surprisingly elegant. It uses three concepts:

- **Packages**: A Cargo project that can build one or more crates
- **Crates**: A tree of modules that produces a library or executable
- **Modules**: Namespaced units of code that control visibility

Let's build a small project to see how these fit together.

## Packages: The Cargo Project

When you run `cargo new my_project`, you create a **package**. A package is just a `Cargo.toml` file and a convention for where source files live. The package can contain:

- At most **one** library crate (`src/lib.rs`)
- As many **binary** crates as you want (`src/main.rs` plus `src/bin/*.rs`)

Let's create a package that will be our running example:

```rust
// File: Cargo.toml
[package]
name = "restaurant"
version = "0.1.0"
edition = "2021"

// This is all you need – Cargo treats src/main.rs or src/lib.rs
// as the crate root automatically
```

## Crates: The Compilation Unit

A **crate** is what the Rust compiler actually compiles. It's a tree of modules. Every crate has a **root module** – that's `src/main.rs` for binaries or `src/lib.rs` for libraries.

```rust
// File: src/lib.rs
// This is the crate root for our library crate.
// Everything defined here is part of this crate's module tree.

/// Public function: available outside the crate
pub fn seat_customers(count: u32) {
    println!("Seating {} customer(s)", count);
    // We can call private functions from within the crate
    prepare_kitchen();
}

/// Private function: only usable within this crate
fn prepare_kitchen() {
    println!("Kitchen is ready!");
}
```

The key insight: **everything is private by default** in Rust. The `pub` keyword is your opt-in to visibility.

## Modules: Namespacing Your Code

Modules let you group related functionality. They can be defined inline or in separate files.

### Inline Modules

```rust
// File: src/lib.rs
// Define a module inline using `mod` keyword

mod front_of_house {
    // Modules create their own scope
    pub mod hosting {
        pub fn add_to_waitlist() {}
        fn seat_at_table() {}  // Private by default
    }

    mod serving {
        fn take_order() {}
        fn serve_order() {}
        fn take_payment() {}
    }
}

// To use something from a module, we use the `use` keyword
use front_of_house::hosting;

pub fn eat_at_restaurant() {
    // Absolute path (starts with crate)
    crate::front_of_house::hosting::add_to_waitlist();
    
    // Relative path (uses `self`, `super`, or just a name)
    front_of_house::hosting::add_to_waitlist();
    
    // Using the `use` import we made above
    hosting::add_to_waitlist();
}
```

### Separating Modules into Files

For real projects, you'll want modules in their own files. Here's the idiomatic way:

```rust
// File: src/lib.rs
// Declare the module – Rust will look for:
// 1. src/front_of_house.rs
// 2. src/front_of_house/mod.rs (older style, still works)
pub mod front_of_house;

// Now we can use it
use front_of_house::hosting;
```

```rust
// File: src/front_of_house.rs
// This file IS the front_of_house module
pub mod hosting;

mod serving {
    // ... server functions
}
```

```rust
// File: src/front_of_house/hosting.rs
// Rust also supports directories-as-modules
// This file lives at src/front_of_house/hosting.rs
pub fn add_to_waitlist() {
    println!("Added to waitlist!");
}
```

The file-system mapping is:
- `mod front_of_house` → `src/front_of_house.rs` or `src/front_of_house/mod.rs`
- `mod hosting` inside `front_of_house` → `src/front_of_house/hosting.rs`

## The `pub` Keyword and Visibility Rules

This is where beginners (including me!) often fight the compiler. Here's the hierarchy of visibility:

| Declaration | Visible in... |
|-------------|---------------|
| `fn foo()` | Current module and children |
| `pub fn foo()` | Everywhere (if the module path is accessible) |
| `pub(crate) fn foo()` | Anywhere in the current crate |
| `pub(super) fn foo()` | Parent module and its children |
| `pub(in path::to::module)` | Specific module and its children |

```rust
// File: src/lib.rs
mod kitchen {
    // This function is usable anywhere in the crate
    pub(crate) fn wash_dishes() {
        println!("Washing...");
    }
    
    // This struct's fields are private by default
    pub struct Chef {
        pub name: String,     // Public field
        years_experience: u8, // Private field
    }
    
    impl Chef {
        // Public constructor – the only way to create a Chef
        // from outside the module
        pub fn new(name: String, years: u8) -> Chef {
            Chef {
                name,
                years_experience: years,
            }
        }
        
        // Public getter for private field
        pub fn experience(&self) -> u8 {
            self.years_experience
        }
    }
}
```

## Compiler Fight: "Module Not Found"

Here's a mistake I made three times before it stuck:

```rust
// File: src/lib.rs
// WRONG – I tried to use the module before declaring it
use menu::appetizers::SpringRolls;

// FIX: declare the module first
pub mod menu;  // Tells Rust to look for src/menu.rs
use menu::appetizers::SpringRolls;
```

The compiler error:
```
error[E0432]: unresolved import `menu::appetizers`
 --> src/lib.rs:2:5
  |
2 | use menu::appetizers::SpringRolls;
  |     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ use of undeclared module `menu`
```

The rule: **declare before use**. `mod menu;` is like telling Rust "there's a module file I want you to compile". Only after that declaration can you `use` items from it.

## Try It Yourself

Create a new package and practice the module system:

```rust
// 1. Run: cargo new restaurant --lib
// 2. Create this structure:
//    src/
//      lib.rs
//      front_of_house/
//        hosting.rs
//        serving.rs

// In src/lib.rs:
pub mod front_of_house;

// In src/front_of_house/hosting.rs:
pub fn add_to_waitlist() {
    println!("Party of {} added!", 2);
}

// In src/front_of_house/serving.rs:
pub fn take_order(item: &str) {
    println!("Order taken: {}", item);
}

// Now in src/lib.rs, try:
use front_of_house::hosting;
use front_of_house::serving;

pub fn run_restaurant() {
    hosting::add_to_waitlist();
    serving::take_order("burger");
}
```

Try making `serving::take_order` private (remove `pub`) and see what happens when you try to call it from `lib.rs`. The compiler will teach you about visibility!

---
*Next up → **Day 12:** Vectors: Growing Lists*