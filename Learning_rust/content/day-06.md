---
title: "Day 06: The Slice Type"
date: 2026-06-24
tags: [til, rust, slices, references, borrowing]
---

# Day 06: The Slice Type

## What I Explored Today

Yesterday I learned about references and borrowing—how to lend data without giving up ownership. Today I discovered **slices**, which are like references but for a *contiguous sequence* of elements rather than a single value. Slices let me work with parts of collections (strings, arrays, vectors) safely and efficiently.

The key insight: slices are **fat pointers**—they store both a pointer to the start of the data *and* a length. This means Rust knows exactly how many elements the slice refers to, preventing out-of-bounds access at compile time where possible and always at runtime.

## String Slices

A string slice is a reference to part of a `String` (or `&str` literal). The type is `&str` (pronounced "string slice").

```rust
fn main() {
    let full_name = String::from("Ada Lovelace");
    
    // Take a slice of the first 3 characters
    let first_name = &full_name[0..3];  // "Ada"
    
    // Take a slice from index 4 to the end
    let last_name = &full_name[4..];    // "Lovelace"
    
    println!("First: {first_name}, Last: {last_name}");
    
    // The original String is still usable (we only borrowed)
    println!("Full: {full_name}");
}
```

### Range Syntax

Rust has flexible range syntax for slicing:

| Syntax | Meaning | Example |
|--------|---------|---------|
| `0..5` | from index 0 up to (but not including) 5 | `"hello world"[0..5]` → `"hello"` |
| `..5` | from start up to index 5 | `"hello world"[..5]` → `"hello"` |
| `2..7` | from index 2 to 7 | `"hello world"[2..7]` → `"llo w"` |
| `2..` | from index 2 to end | `"hello world"[2..]` → `"llo world"` |
| `..` | the entire string | `"hello world"[..]` → `"hello world"` |

**Important:** String slices must occur on valid UTF-8 character boundaries. If you try to slice in the middle of a multi-byte character (like an emoji), your program will panic at runtime.

```rust
fn main() {
    let emoji_string = String::from("Hello 😊 world");
    
    // This works—'😊' is 4 bytes, starting at index 6
    let valid_slice = &emoji_string[6..10];  // "😊"
    println!("Valid: {valid_slice}");
    
    // This would PANIC—slicing in the middle of the emoji bytes
    // let bad_slice = &emoji_string[6..8];  // Compiles but panics at runtime!
}
```

## A Practical Example: First Word Function

Here's where slices really shine—writing a function that extracts the first word from a string:

```rust
// Bad approach: returns an index, caller must coordinate
fn first_word_bad(s: &String) -> usize {
    let bytes = s.as_bytes();
    
    for (i, &byte) in bytes.iter().enumerate() {
        if byte == b' ' {  // Found a space
            return i;
        }
    }
    
    s.len()  // No space found, whole string is one word
}

// Good approach: returns a string slice
fn first_word(s: &str) -> &str {
    let bytes = s.as_bytes();
    
    for (i, &byte) in bytes.iter().enumerate() {
        if byte == b' ' {
            return &s[0..i];  // Return a slice up to the space
        }
    }
    
    &s[..]  // No space found, return whole string
}

fn main() {
    let mut sentence = String::from("Hello world");
    
    // With the bad version, this is dangerous:
    let word_end = first_word_bad(&sentence);
    sentence.clear();  // Now word_end (5) is meaningless!
    // word_end still exists but refers to nothing useful
    
    // With the slice version, the borrow checker saves us:
    let word = first_word(&sentence);  // immutable borrow of sentence
    // sentence.clear();  // COMPILE ERROR! Can't mutate while borrowed
    println!("First word: {word}");  // This works fine
}
```

Notice the function signature: `fn first_word(s: &str) -> &str`. Using `&str` as the parameter type is more flexible—it accepts both string slices (`&str`) and references to `String` (`&String` gets automatically coerced to `&str`).

## Array Slices

Slices aren't just for strings—they work with any collection:

```rust
fn main() {
    let numbers = [1, 2, 3, 4, 5, 6, 7, 8];
    
    // Create a slice of the first 4 elements
    let first_half = &numbers[0..4];    // type: &[i32]
    let second_half = &numbers[4..];    // type: &[i32]
    
    println!("First half: {:?}", first_half);
    println!("Second half: {:?}", second_half);
    
    // Slices implement common traits
    println!("Sum of first half: {}", first_half.iter().sum::<i32>());
    println!("Length of second half: {}", second_half.len());
}
```

## Compiler Fight: Mutable and Immutable References

Slices are references, so they follow the same borrowing rules. Here's a common fight:

```rust
fn main() {
    let mut text = String::from("Hello, world!");
    
    let word = &text[0..5];  // immutable borrow starts here
    
    text.push_str("!!!");    // ERROR: mutable borrow while immutable exists
    
    println!("{word}");      // immutable borrow used here
}
```

The compiler says:
```
error[E0502]: cannot borrow `text` as mutable because it is also borrowed as immutable
```

**The fix:** Reorder the operations so the immutable borrow ends before the mutation:

```rust
fn main() {
    let mut text = String::from("Hello, world!");
    
    let word = &text[0..5];  // immutable borrow
    println!("{word}");      // immutable borrow ends here
    
    text.push_str("!!!");    // Now we can mutate
    println!("{text}");      // "Hello, world!!!!"
}
```

## Other Slice Tricks

Slices can be sliced too! And you can get sub-slices:

```rust
fn main() {
    let data = [10, 20, 30, 40, 50, 60];
    
    let slice1 = &data[1..4];      // [20, 30, 40]
    let slice2 = &slice1[0..2];    // [20, 30] — slice of a slice!
    
    println!("{:?}", slice2);
    
    // You can even get a slice of a String literal
    let greeting = "Hello, world!";
    let hello = &greeting[..5];     // "Hello"
    println!("{hello}");
}
```

## Try It Yourself

Write a function `second_word` that returns the second word from a string. If the string has fewer than two words, return an empty slice. Then test it:

```rust
fn second_word(s: &str) -> &str {
    // Your code here
}

fn main() {
    let phrase = String::from("Rust is fun");
    let second = second_word(&phrase);
    println!("Second word: '{second}'");  // Should print: 'is'
    
    let short = "Hello";
    let second_short = second_word(short);
    println!("Second word (short): '{second_short}'");  // Should print: ''
}
```

**Hint:** You can call `first_word` inside `second_word` and use the index it returns. Or iterate through bytes looking for space characters.

---
*Next up → **Day 07:** Structs: Defining & Instantiating — we'll create our own custom data types!*