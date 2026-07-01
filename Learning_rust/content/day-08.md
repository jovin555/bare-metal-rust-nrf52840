---
title: "Day 08: Methods on Structs"
date: 2026-06-24
tags: [til, rust, structs, methods, impl-blocks]
---

# Day 08: Methods on Structs

## What I Explored Today

Yesterday I learned how to define structs and create instances. Today I discovered how to bring those structs to life by attaching **methods** — functions that belong to a specific struct type.

Methods are defined inside an `impl` (implementation) block, and they always have access to the struct's data through `self`. This is Rust's version of "object-oriented" programming, but with a twist: methods and data are kept separate by design.

## The Anatomy of a Method

Let's start with a simple `Rectangle` struct and add methods to calculate area and check if it can hold another rectangle:

```rust
// Define a struct with two fields
struct Rectangle {
    width: u32,
    height: u32,
}

// impl block — everything here is attached to Rectangle
impl Rectangle {
    // &self is a reference to the struct instance
    // We borrow immutably because we only read data
    fn area(&self) -> u32 {
        self.width * self.height
    }

    // Another method taking &self
    fn can_hold(&self, other: &Rectangle) -> bool {
        // We can access fields of both self and other
        self.width > other.width && self.height > other.height
    }

    // A method that takes ownership of self (rare but useful)
    fn consume(self) {
        println!("Consumed rectangle {}x{}", self.width, self.height);
        // self is dropped here — can't use the instance after this
    }
}

fn main() {
    let rect1 = Rectangle {
        width: 30,
        height: 50,
    };
    let rect2 = Rectangle {
        width: 10,
        height: 40,
    };

    // Call methods using dot notation — just like other languages
    println!("Area of rect1: {}", rect1.area());
    println!("Can rect1 hold rect2? {}", rect1.can_hold(&rect2));
    println!("Can rect2 hold rect1? {}", rect2.can_hold(&rect1));

    // consume() takes ownership
    rect1.consume();
    // println!("Area: {}", rect1.area()); // ERROR: rect1 is gone!
}
```

## Three Flavors of `self`

Rust gives us three ways to accept `self` in a method, each with different ownership semantics:

```rust
struct Counter {
    value: i32,
}

impl Counter {
    // 1. &self — borrows immutably (most common)
    // Use when you only need to read data
    fn get_value(&self) -> i32 {
        self.value
    }

    // 2. &mut self — borrows mutably
    // Use when you need to change the struct's data
    fn increment(&mut self) {
        self.value += 1;
    }

    // 3. self — takes ownership
    // Use when you want to consume the instance
    fn into_string(self) -> String {
        format!("Counter value: {}", self.value)
        // self is moved into this method and dropped here
    }
}

fn main() {
    // Need mut to call &mut self methods
    let mut counter = Counter { value: 0 };

    println!("Initial: {}", counter.get_value());
    counter.increment();
    counter.increment();
    println!("After increments: {}", counter.get_value());

    // Consume the counter
    let description = counter.into_string();
    println!("{}", description);
    // println!("{}", counter.get_value()); // ERROR: counter is moved
}
```

## Associated Functions (No `self`)

Not everything in an `impl` block needs `self`. Functions without `self` are called **associated functions** — they're like static methods in other languages. The most common one is a constructor:

```rust
struct Point {
    x: f64,
    y: f64,
}

impl Point {
    // Associated function — no self parameter
    // Called with Point::new(...) instead of instance.new()
    fn new(x: f64, y: f64) -> Point {
        Point { x, y }  // field init shorthand works here too
    }

    // Another associated function
    fn origin() -> Point {
        Point { x: 0.0, y: 0.0 }
    }

    // Regular method — needs an instance
    fn distance_from(&self, other: &Point) -> f64 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        (dx * dx + dy * dy).sqrt()
    }
}

fn main() {
    // Call associated functions with ::
    let p1 = Point::new(3.0, 4.0);
    let origin = Point::origin();

    // Call methods with .
    let dist = p1.distance_from(&origin);
    println!("Distance: {:.2}", dist);  // 5.00
}
```

## Multiple `impl` Blocks

You can have multiple `impl` blocks for the same struct. This is great for organization:

```rust
struct User {
    name: String,
    age: u8,
}

// First impl block — basic getters
impl User {
    fn name(&self) -> &str {
        &self.name
    }

    fn age(&self) -> u8 {
        self.age
    }
}

// Second impl block — validation logic
impl User {
    fn is_adult(&self) -> bool {
        self.age >= 18
    }

    fn can_retire(&self) -> bool {
        self.age >= 65
    }
}

fn main() {
    let user = User {
        name: String::from("Alice"),
        age: 30,
    };

    println!("{} is {} years old", user.name(), user.age());
    println!("Adult: {}", user.is_adult());
    println!("Can retire: {}", user.can_retire());
}
```

## Fighting the Compiler

Here's a classic mistake: trying to call a method that takes `&self` after calling one that takes `self`:

```rust
struct Data {
    value: String,
}

impl Data {
    fn show(&self) {
        println!("Value: {}", self.value);
    }

    fn consume(self) {
        println!("Consumed: {}", self.value);
    }
}

fn main() {
    let data = Data {
        value: String::from("important"),
    };

    data.show();     // ✓ borrows immutably
    data.consume();  // ✓ takes ownership
    // data.show();  // ✗ COMPILE ERROR: value borrowed after move
}
```

**Error message:**
```
error[E0382]: borrow of moved value: `data`
  --> src/main.rs:19:5
   |
16 |     data.consume();
   |     ---- value moved here
17 |     data.show();
   |     ^^^^^^^^^^^ value borrowed here after move
```

**Fix:** Either don't call `consume()` if you need the data later, or restructure your code to use the data before consuming it.

## Try It Yourself

Create a `BankAccount` struct with:
- Fields: `owner: String` and `balance: f64`
- An associated function `new(owner: &str)` that creates an account with $0 balance
- Methods: `deposit(&mut self, amount: f64)`, `withdraw(&mut self, amount: f64)` (return `bool` for success), and `check_balance(&self) -> f64`

```rust
// Your turn!
struct BankAccount {
    owner: String,
    balance: f64,
}

impl BankAccount {
    // Write your implementation here
}

fn main() {
    let mut account = BankAccount::new("Bob");
    account.deposit(500.0);
    account.withdraw(200.0);
    println!("Balance: ${:.2}", account.check_balance());
}
```

---
*Next up → **Day 09:** Enums & Pattern Matching with `match`*