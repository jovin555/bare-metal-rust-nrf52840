//! Day 1 — Toolchain Setup and First Flash
//!
//! The simplest possible bare-metal Rust program for the nRF52840.
//! It does nothing — but proving it builds and runs is the whole point of Day 1.
//!
//! Flash with:  cargo run --release
//! Build only:  cargo build --release

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use panic_halt as _;

#[entry]
fn main() -> ! {
    // Spin forever. The CPU is running bare-metal Rust code on the nRF52840.
    loop {
        cortex_m::asm::nop();
    }
}
