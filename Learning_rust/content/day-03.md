---
title: "Day 03: Control Flow: if, loop, while, for"
date: 2026-06-24
tags: [til, control-flow, loops, conditionals]
---

# Day 03: Control Flow: if, loop, while, for

## What I Explored Today

Yesterday I learned how to store and manipulate data with variables. Today I'm giving my programs a brain — the ability to make decisions and repeat actions. Rust's control flow is where the language starts to feel different from other languages I've used. The syntax is familiar, but some of the rules (like `if` being an expression that returns a value) took me a moment to wrap my head around.

## Conditionals: `if` is an Expression

In many languages, `if` is a *statement* — it does something but doesn't produce a value. In Rust, `if` is an *expression* that can return a value. This means I can assign the result of an `if` directly to a variable.

```rust
fn main() {
    let temperature = 22; // degrees Celsius
    
    // Classic if-else, looks like C/Java
    if temperature > 30 {
        println!("It's hot outside!");
    } else if temperature > 20 {
        println!("It's a pleasant day.");
    } else {
        println!("It's a bit chilly.");
    }
    
    // Here's the Rust twist: if is an EXPRESSION
    // Both arms must return the same type
    let weather_description = if temperature > 25 {
        "warm"   // no semicolon = this is the return value
    } else if temperature > 15 {
        "mild"
    } else {
        "cool"
    };
    // ^ semicolon ends the let statement, not the if
    
    println!("Today feels {}.", weather_description);
}
```

**Key insight**: When using `if` as an expression, every arm must return the same type. If one arm returns a string and another returns a number, Rust won't compile it.

## The `loop` Keyword: Infinite by Design

Rust has three kinds of loops, and `loop` is the most unique. It creates an infinite loop by default — no condition needed. This sounds dangerous, but Rust gives us tools to escape: `break` and `continue`.

```rust
fn main() {
    let mut counter = 0;
    
    // loop runs forever unless we break out
    let result = loop {
        counter += 1;
        
        if counter == 10 {
            // break can return a value from the loop!
            break counter * 2;
        }
    };
    
    println!("The loop returned: {}", result); // 20
    
    // Nested loops need labels to break the right one
    let mut matrix_sum = 0;
    'outer: loop {
        'inner: loop {
            matrix_sum += 1;
            if matrix_sum >= 5 {
                break 'outer; // breaks the OUTER loop, not inner
            }
        }
    }
    println!("Broke out of outer loop at sum = {}", matrix_sum);
}
```

The ability to `break` with a value is a Rust-specific feature I find elegant. Instead of mutating a variable outside the loop and then checking it, I can compute and return the result directly.

## `while` Loops: Conditional Repetition

The `while` loop works like most languages — it repeats as long as a condition is true. It's syntactic sugar over `loop` with an `if`-`break` pattern.

```rust
fn main() {
    let mut number = 3;
    
    while number > 0 {
        println!("{}!", number);
        number -= 1;
    }
    println!("Liftoff! 🚀");
    
    // Common pattern: looping through a collection
    let fruits = ["apple", "banana", "cherry"];
    let mut index = 0;
    
    while index < fruits.len() {
        println!("I like {}", fruits[index]);
        index += 1;
    }
}
```

But wait — the `while` loop with an index is actually a Rust anti-pattern. There's a better way...

## `for` Loops: The Idiomatic Choice

The `for` loop in Rust is designed for iterating over collections. It's safer and more expressive than manual indexing with `while`.

```rust
fn main() {
    let fruits = ["apple", "banana", "cherry"];
    
    // This is the Rust way — no index variable, no bounds checking
    for fruit in fruits {
        println!("I like {}", fruit);
    }
    
    // Range syntax: start..end (exclusive end)
    for number in 1..4 {
        println!("{}!", number);
    }
    // Prints: 1! 2! 3!
    
    // Range with =end (inclusive end)
    for number in 1..=3 {
        println!("{}!", number);
    }
    // Prints: 1! 2! 3!
    
    // Getting the index too with .enumerate()
    for (index, fruit) in fruits.iter().enumerate() {
        println!("Fruit #{} is {}", index + 1, fruit);
    }
    
    // Reverse a range
    for number in (1..4).rev() {
        println!("{}!", number);
    }
    // Prints: 3! 2! 1!
}
```

## Compiler Fight: Mismatched Types in `if` Expression

Here's a mistake I made that the compiler caught immediately:

```rust
fn main() {
    let condition = true;
    
    // This looks reasonable to a beginner...
    let number = if condition {
        5
    } else {
        "six"  // ERROR: expected integer, found &str
    };
}
```

**The error:**
```
error[E0308]: `if` and `else` have incompatible types
 --> src/main.rs:6:9
  |
4 |       let number = if condition {
5 |           5
  |           - expected because of this
6 |           "six"
  |           ^^^^^ expected integer, found `&str`
```

**The fix:** Both arms must return the same type. Either make both integers or both strings.

```rust
fn main() {
    let condition = true;
    
    // Fixed version — both arms return i32
    let number = if condition {
        5
    } else {
        6  // now both arms are integers
    };
    
    println!("The number is: {}", number);
}
```

## Try It Yourself

```rust
fn main() {
    // Challenge: Print "Fizz" for multiples of 3,
    // "Buzz" for multiples of 5,
    // "FizzBuzz" for multiples of both,
    // and the number otherwise.
    // Hint: Use a for loop from 1..=30
    
    for n in 1..=30 {
        if n % 15 == 0 {
            println!("FizzBuzz");
        } else if n % 3 == 0 {
            println!("Fizz");
        } else if n % 5 == 0 {
            println!("Buzz");
        } else {
            println!("{}", n);
        }
    }
}
```

**Bonus thought:** Notice how `if` as an expression means I could write this FizzBuzz logic as a single expression that returns a string for each `n`. Try refactoring it that way!

---
*Next up → **Day 04:** Ownership: The Core Mental Model — the concept that makes Rust unique*