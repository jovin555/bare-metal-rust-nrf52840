---
title: "Day 09: Enums & Pattern Matching with match"
date: 2026-06-24
tags: [til, rust, enums, pattern-matching]
---

# Day 09: Enums & Pattern Matching with match

## What I Explored Today

Today I learned about **enums** — one of Rust's most powerful features for modeling data that can take multiple forms. Combined with `match`, Rust's pattern matching construct, enums let you write code that's both expressive and safe. The compiler forces you to handle every possible variant, which eliminates entire categories of bugs.

## Defining Enums: Grouping Related Values

An **enum** (short for "enumeration") defines a type that can be one of several variants. Think of it like a menu of choices — a value can only be one option at a time.

```rust
// Define an enum for IP addresses
enum IpAddrKind {
    V4,
    V6,
}

fn main() {
    // Create instances of each variant
    let four = IpAddrKind::V4;
    let six = IpAddrKind::V6;

    // We can pass them to functions
    route(four);
    route(six);
}

fn route(ip_kind: IpAddrKind) {
    // Function takes any IpAddrKind
    println!("Routing an IP address kind");
}
```

The key insight: `IpAddrKind::V4` and `IpAddrKind::V6` share the same type, so functions can accept any variant. This is much cleaner than using separate constants or strings.

## Enums with Data: Variants That Carry Values

Here's where enums get really powerful — each variant can hold data of different types and amounts:

```rust
// Each variant can carry different data
enum Message {
    Quit,                       // No data
    Move { x: i32, y: i32 },   // Named fields like a struct
    Write(String),              // A single String
    ChangeColor(i32, i32, i32), // Three i32 values
}

fn main() {
    // Create different messages
    let msg1 = Message::Quit;
    let msg2 = Message::Move { x: 10, y: 20 };
    let msg3 = Message::Write(String::from("Hello!"));
    let msg4 = Message::ChangeColor(255, 0, 0);
}
```

This is more compact than defining four separate structs. The `Message` type encompasses all four possibilities, and we can write functions that work with any message.

## Introducing `match`: Exhaustive Pattern Matching

The `match` construct is like a super-powered `switch` statement. It compares a value against patterns and runs the code for the first matching pattern. The crucial safety feature: **the compiler checks that you've covered all possible variants**.

```rust
enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter,
}

fn value_in_cents(coin: Coin) -> u8 {
    // Match each variant to a value
    match coin {
        Coin::Penny => 1,        // Simple: return value
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter => 25,     // Last arm, no comma needed
    }
}

fn main() {
    println!("A dime is worth {} cents", value_in_cents(Coin::Dime));
    // Output: A dime is worth 10 cents
}
```

If I forget a variant, the compiler won't let me compile:

```rust
// ❌ COMPILER ERROR: non-exhaustive patterns
fn value_in_cents_incomplete(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        // Missing Dime and Quarter!
    }
}
```

## Matching with Data: Destructuring Values

When enum variants carry data, `match` lets us **destructure** that data — extracting the values inside:

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

fn handle_message(msg: Message) {
    match msg {
        Message::Quit => {
            println!("Goodbye!");
        }
        // Destructure named fields
        Message::Move { x, y } => {
            println!("Moving to ({}, {})", x, y);
        }
        // Destructure a tuple variant
        Message::Write(text) => {
            println!("Message: {}", text);
        }
        // Destructure multiple values
        Message::ChangeColor(r, g, b) => {
            println!("New color: RGB({}, {}, {})", r, g, b);
        }
    }
}

fn main() {
    let msg = Message::Move { x: 30, y: 50 };
    handle_message(msg);
    // Output: Moving to (30, 50)
}
```

The variables `x`, `y`, `text`, `r`, `g`, `b` are bound to the data inside each variant. This is Rust's way of safely unpacking values — you can only access the data if the variant matches.

## The Catch-All Pattern: `_` and `other`

Sometimes you only care about a few variants. Use `_` as a catch-all for the rest:

```rust
fn describe_digit(digit: u8) -> &'static str {
    match digit {
        0 => "zero",
        1 => "one",
        2 => "two",
        3..=9 => "three to nine",  // Range pattern
        _ => "not a single digit",  // Catch-all for 10+
    }
}

fn main() {
    println!("5 is {}", describe_digit(5));   // three to nine
    println!("0 is {}", describe_digit(0));   // zero
    println!("42 is {}", describe_digit(42)); // not a single digit
}
```

The `_` pattern matches anything. You can also name the catch-all variable:

```rust
fn coin_color(coin: Coin) -> &'static str {
    match coin {
        Coin::Penny => "copper",
        other => {  // 'other' binds to the value
            println!("Not a penny, it's something else");
            "silver"
        }
    }
}
```

## Compiler Fight: Forgetting the Catch-All

Here's a classic mistake I made today. I tried to match on a `u8` but only handled specific values:

```rust
fn main() {
    let number = 42;

    match number {
        1 => println!("One!"),
        2 => println!("Two!"),
        3 => println!("Three!"),
        // ERROR: non-exhaustive patterns
        // What about 0, 4, 5, ... 255?
    }
}
```

The compiler error:
```
error[E0004]: non-exhaustive patterns: `0u8` and `0u8..=255u8` not covered
```

The fix is simple — add a catch-all:

```rust
match number {
    1 => println!("One!"),
    2 => println!("Two!"),
    3 => println!("Three!"),
    _ => println!("Something else"), // Now all values handled
}
```

This might seem annoying, but it's a feature! It prevents you from forgetting to handle a case when you add a new enum variant later.

## Real-World Example: A Simple State Machine

Let's combine everything into a practical example — modeling a traffic light:

```rust
enum TrafficLight {
    Red,
    Yellow,
    Green,
}

impl TrafficLight {
    fn next(&self) -> TrafficLight {
        match self {
            TrafficLight::Red => TrafficLight::Green,
            TrafficLight::Yellow => TrafficLight::Red,
            TrafficLight::Green => TrafficLight::Yellow,
        }
    }

    fn description(&self) -> &str {
        match self {
            TrafficLight::Red => "Stop!",
            TrafficLight::Yellow => "Prepare to stop...",
            TrafficLight::Green => "Go!",
        }
    }
}

fn main() {
    let mut light = TrafficLight::Red;
    println!("Light is red: {}", light.description());

    light = light.next();
    println!("Now green: {}", light.description());

    light = light.next();
    println!("Now yellow: {}", light.description());

    light = light.next();
    println!("Back to red: {}", light.description());
}
```

The compiler guarantees we handle all three states. If we add a `Flashing` variant later, every `match` will break until we update it — no runtime surprises.

## Try It Yourself

Write a function that takes a `Temperature` enum (with `Celsius(f64)` and `Fahrenheit(f64)` variants) and converts it to the other scale. Use `match` to handle both variants. Remember: `C = (F - 32) * 5/9` and `F = C * 9/5 + 32`.

```rust
enum Temperature {
    Celsius(f64),
    Fahrenheit(f64),
}

fn convert(temp: Temperature) -> Temperature {
    // Your code here
}

fn main() {
    let boiling = Temperature::Celsius(100.0);
    let converted = convert(boiling);
    // Should print: 212.0°F
    match converted {
        Temperature::Fahrenheit(f) => println!("{}°F", f),
        _ => println!("Conversion failed"),
    }
}
```

---
*Next up → **Day 10:** Option<T> — Handling the Absence of a Value*