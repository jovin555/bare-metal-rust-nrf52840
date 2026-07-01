//! Day 2 — Blinky with the PAC (Peripheral Access Crate)
//!
//! Blinks LED1 (P0.13, active-low) on the nRF52840 DK by writing
//! directly to GPIO registers via the generated PAC — no HAL, no abstractions.
//!
//! nRF52840 DK LEDs (all active-low):
//!   LED1 = P0.13   LED2 = P0.14   LED3 = P0.15   LED4 = P0.16

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use nrf52840_pac as pac;
use panic_halt as _;

const LED1_PIN: u32 = 13;

#[entry]
fn main() -> ! {
    let p = pac::Peripherals::take().unwrap();
    let p0 = &p.P0;

    // Configure P0.13 as push-pull output, initially high (LED off)
    p0.pin_cnf[LED1_PIN as usize].write(|w| {
        w.dir().output();
        w.input().disconnect();
        w.pull().disabled();
        w.drive().s0s1();
        w.sense().disabled()
    });

    // Drive high so the LED starts off
    p0.outset.write(|w| unsafe { w.bits(1 << LED1_PIN) });

    loop {
        // Clear bit → pin low → LED on
        p0.outclr.write(|w| unsafe { w.bits(1 << LED1_PIN) });
        cortex_m::asm::delay(8_000_000); // ~1 s at 64 MHz

        // Set bit → pin high → LED off
        p0.outset.write(|w| unsafe { w.bits(1 << LED1_PIN) });
        cortex_m::asm::delay(8_000_000);
    }
}
