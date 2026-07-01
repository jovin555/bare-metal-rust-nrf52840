//! Day 3 — Blinky with the HAL (Hardware Abstraction Layer)
//!
//! Same LED blink as Day 2, but using nrf52840-hal safe abstractions.
//! The type system enforces correct pin direction at compile time.
//!
//! Compare with Day 2: no unsafe, no raw bit manipulation, no register offsets.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use nrf52840_hal::{
    gpio::{p0::Parts, Level},
    pac,
    prelude::*,
};
use panic_halt as _;

#[entry]
fn main() -> ! {
    let p = pac::Peripherals::take().unwrap();
    let port0 = Parts::new(p.P0);

    // Type system guarantees this pin is an output — calling set_low on an
    // input pin would be a compile error.
    let mut led1 = port0.p0_13.into_push_pull_output(Level::High); // High = LED off

    loop {
        led1.set_low().unwrap(); // LED on
        cortex_m::asm::delay(8_000_000);

        led1.set_high().unwrap(); // LED off
        cortex_m::asm::delay(8_000_000);
    }
}
