---
title: "Day 07: Structs: Defining & Instantiating"
date: 2026-06-24
tags: [til, rust, structs, data-structures]
---

# Day 07: Structs: Defining & Instantiating

## What I Explored Today

So far, I've been working with tuples and arrays to group related data. But tuples have a limitation: you access elements by position (`tuple.0`, `tuple.1`), which gets confusing when you have more than a few fields. Today I learned about **structs**—Rust's way of creating custom data types with named fields. Think of structs like a form with labeled blanks: you know exactly what each piece of data means.

## Defining a Struct

A struct is defined with the `struct` keyword, followed by a name (PascalCase by convention) and curly braces containing named fields.

```rust
// Define a struct to represent a user account
struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}
```

Each field has a name and a type. The order doesn't matter—you access fields by name, not position. This is the key advantage over tuples: clarity.

## Instantiating a Struct

To create an instance of a struct, you specify the struct name and provide values for every field in curly braces:

```rust
fn main() {
    // Create an instance of User
    let user1 = User {
        email: String::from("alice@example.com"),
        username: String::from("alice123"),
        active: true,
        sign_in_count: 1,
    };

    // Access fields with dot notation
    println!("User: {}", user1.username);
    println!("Email: {}", user1.email);
    println!("Active: {}", user1.active);
    println!("Sign-ins: {}", user1.sign_in_count);
}
```

**Important:** All fields must be provided when creating an instance. Rust won't let you leave anything uninitialized—that would be undefined behavior.

## Mutable Structs

By default, struct instances are immutable. To change a field, you need to mark the entire instance as `mut`:

```rust
fn main() {
    let mut user2 = User {
        email: String::from("bob@example.com"),
        username: String::from("bob456"),
        active: true,
        sign_in_count: 1,
    };

    // This works because user2 is mutable
    user2.email = String::from("bob_new@example.com");
    
    // But this would fail if user2 weren't mut
    // user2.sign_in_count += 1; // ERROR: cannot borrow `user2` as mutable

    println!("Updated email: {}", user2.email);
}
```

**Note:** Rust doesn't allow marking individual fields as mutable—it's all or nothing for the whole instance.

## The Field Init Shorthand

If you have variables with the same names as struct fields, you can use the shorthand syntax:

```rust
fn build_user(email: String, username: String) -> User {
    User {
        email,  // shorthand for email: email
        username,  // shorthand for username: username
        active: true,
        sign_in_count: 1,
    }
}
```

This is incredibly common in Rust code. When the variable name matches the field name exactly, you can just write the name once.

## Struct Update Syntax

To create a new instance that's mostly the same as an existing one, use `..` to copy remaining fields:

```rust
fn main() {
    let user1 = User {
        email: String::from("alice@example.com"),
        username: String::from("alice123"),
        active: true,
        sign_in_count: 1,
    };

    // Create user2 with a different email, but same everything else
    let user2 = User {
        email: String::from("alice_work@example.com"),
        ..user1  // fill remaining fields from user1
    };

    // Wait... what happened to user1's username?
    // println!("{}", user1.username); // ERROR! value moved!
}
```

**⚠️ Ownership trap:** The `..user1` syntax *moves* the fields that implement `Copy` (like `bool` and `u64`) but *moves ownership* of fields like `String`. After creating `user2`, `user1.username` is no longer valid because its `String` was moved. However, `user1.active` and `user1.sign_in_count` are still usable because `bool` and `u64` implement `Copy`.

## Tuple Structs

Sometimes you want the naming of a struct but without named fields—just positions like a tuple. Enter **tuple structs**:

```rust
// Define tuple structs
struct Color(i32, i32, i32);
struct Point(i32, i32, i32);

fn main() {
    let black = Color(0, 0, 0);
    let origin = Point(0, 0, 0);

    // These are DIFFERENT types, even though they have the same structure
    // let error: Point = black; // ERROR: expected `Point`, found `Color`

    // Access by position
    println!("Red channel: {}", black.0);
    println!("X coordinate: {}", origin.1);
}
```

The key benefit: `Color` and `Point` are distinct types even though they store the same data. This prevents accidentally passing a color where a coordinate is expected.

## Unit-Like Structs

You can also define structs with no fields at all:

```rust
struct AlwaysEqual;

fn main() {
    let subject = AlwaysEqual;
    // This is useful for implementing traits on types that don't need data
}
```

These are called "unit-like" because they behave like the unit type `()`. They're used when you need to implement a trait but don't need to store any data.

## The Compiler Fight: Missing Fields

Let's see what happens when you forget a field:

```rust
struct Book {
    title: String,
    author: String,
    pages: u32,
}

fn main() {
    let book = Book {
        title: String::from("Rust Programming"),
        // Oops, forgot author and pages!
    };
}
```

**The compiler says:**
```
error[E0063]: missing fields `author` and `pages` in initializer of `Book`
 --> src/main.rs:8:17
  |
8 |     let book = Book {
  |                 ^^^^ missing `author`, `pages`
```

Rust is strict—every field must be initialized. This is a feature, not a bug. It prevents the kind of bugs where you forget to set a field and later read uninitialized data.

## Try It Yourself

Create a struct called `Rectangle` with `width` and `height` fields (both `u32`). Then write a function that takes a `Rectangle` and prints its area.

```rust
// Your code here:
struct Rectangle {
    width: u32,
    height: u32,
}

fn main() {
    let rect = Rectangle {
        width: 30,
        height: 50,
    };
    
    // TODO: Calculate and print the area
    // Hint: width * height
}
```

**Bonus challenge:** Create a `Square` using a tuple struct with one field, then write code that creates a `Rectangle` from it (where width and height are both the square's side length).

---
*Next up → **Day 08:** Methods on Structs — attaching behavior to your custom types*