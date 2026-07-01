//! Day 9 — Hardware Timers
//!
//! Uses Timer0 and Timer1 (TIMER peripheral, 1 MHz base clock) to drive
//! two independent LED blink rates without tying up SysTick.
//!
//! Timer0 → LED1 every 250 ms
//! Timer1 → LED2 every 750 ms
//!
//! The nb crate's block! macro spins until the timer fires, making the
//! CountDown interface simple to use in a sequential loop.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use defmt::info;
use defmt_rtt as _;
use nb::block;
use nrf52840_hal::{
    gpio::{p0::Parts, Level},
    pac,
    prelude::*,
    timer::Timer,
};
use panic_probe as _;

// 1 MHz timer tick → 1 count = 1 µs
const MS_250: u32 = 250_000;
const MS_750: u32 = 750_000;

#[entry]
fn main() -> ! {
    info!("Day 9: Hardware timer demo");

    let p = pac::Peripherals::take().unwrap();
    let port0 = Parts::new(p.P0);

    let mut led1 = port0.p0_13.into_push_pull_output(Level::High);
    let mut led2 = port0.p0_14.into_push_pull_output(Level::High);

    let mut timer0 = Timer::new(p.TIMER0);
    let mut timer1 = Timer::new(p.TIMER1);

    let mut led1_on = false;
    let mut led2_on = false;
    let mut led2_tick = 0u32;

    info!("LED1: 250 ms period | LED2: 750 ms period");

    loop {
        // Wait for the 250 ms timer
        timer0.start(MS_250);
        block!(timer0.wait()).unwrap();

        // Toggle LED1 every 250 ms
        led1_on = !led1_on;
        if led1_on {
            led1.set_low().unwrap();
        } else {
            led1.set_high().unwrap();
        }

        // Toggle LED2 every third 250 ms tick (= 750 ms)
        led2_tick += 1;
        if led2_tick >= 3 {
            led2_tick = 0;
            led2_on = !led2_on;
            if led2_on {
                led2.set_low().unwrap();
                info!("LED2 on");
            } else {
                led2.set_high().unwrap();
                info!("LED2 off");
            }
        }
    }
}
