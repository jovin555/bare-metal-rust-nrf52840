//! Day 6 — GPIO Input: Reading Buttons
//!
//! Reads the four buttons on the nRF52840 DK and drives the four LEDs:
//!   BTN1 (P0.11) → LED1 (P0.13)
//!   BTN2 (P0.12) → LED2 (P0.14)
//!   BTN3 (P0.24) → LED3 (P0.15)
//!   BTN4 (P0.25) → LED4 (P0.16)
//!
//! All buttons and LEDs are active-low. Buttons have internal pull-ups enabled.
//! This is a polling approach — Day 7 will replace the poll loop with interrupts.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use defmt::info;
use defmt_rtt as _;
use embedded_hal::digital::v2::InputPin;
use nrf52840_hal::{
    gpio::{p0::Parts, Level},
    pac,
    prelude::*,
};
use panic_probe as _;

#[entry]
fn main() -> ! {
    info!("Day 6: GPIO button polling demo");

    let p = pac::Peripherals::take().unwrap();
    let port0 = Parts::new(p.P0);

    // Outputs — LEDs (active-low: set_low = on, set_high = off)
    let mut led1 = port0.p0_13.into_push_pull_output(Level::High);
    let mut led2 = port0.p0_14.into_push_pull_output(Level::High);
    let mut led3 = port0.p0_15.into_push_pull_output(Level::High);
    let mut led4 = port0.p0_16.into_push_pull_output(Level::High);

    // Inputs — buttons (active-low: is_low() = pressed)
    let btn1 = port0.p0_11.into_pullup_input();
    let btn2 = port0.p0_12.into_pullup_input();
    let btn3 = port0.p0_24.into_pullup_input();
    let btn4 = port0.p0_25.into_pullup_input();

    loop {
        // Mirror each button state to its LED
        if btn1.is_low().unwrap() {
            led1.set_low().unwrap();
        } else {
            led1.set_high().unwrap();
        }

        if btn2.is_low().unwrap() {
            led2.set_low().unwrap();
        } else {
            led2.set_high().unwrap();
        }

        if btn3.is_low().unwrap() {
            led3.set_low().unwrap();
        } else {
            led3.set_high().unwrap();
        }

        if btn4.is_low().unwrap() {
            led4.set_low().unwrap();
        } else {
            led4.set_high().unwrap();
        }

        // Small delay to reduce CPU load and debounce mechanical bounce
        cortex_m::asm::delay(100_000);
    }
}
