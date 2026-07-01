//! Day 4 — Real Delays with SysTick
//!
//! Uses nrf52840-hal's Delay driver (backed by the Cortex-M SysTick timer)
//! to produce accurate millisecond delays. Also starts the external 32 MHz
//! HFXO so the CPU runs at its rated 64 MHz frequency.
//!
//! Blink pattern: LED1 fast (200 ms), LED2 slow (1000 ms) — simultaneously
//! managed by counting elapsed blinks.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use embedded_hal::blocking::delay::DelayMs;
use nrf52840_hal::{
    clocks::Clocks,
    delay::Delay,
    gpio::{p0::Parts, Level},
    pac,
};
use panic_halt as _;

#[entry]
fn main() -> ! {
    let p = pac::Peripherals::take().unwrap();
    let core = pac::CorePeripherals::take().unwrap();

    // Start the external 32 MHz crystal so HFCLK is accurate
    let _clocks = Clocks::new(p.CLOCK).enable_ext_hfosc();

    let mut delay = Delay::new(core.SYST);

    let port0 = Parts::new(p.P0);
    let mut led1 = port0.p0_13.into_push_pull_output(Level::High);
    let mut led2 = port0.p0_14.into_push_pull_output(Level::High);

    let mut tick = 0u32;

    loop {
        // LED1 toggles every 200 ms
        if tick % 2 == 0 {
            led1.set_low().unwrap();
        } else {
            led1.set_high().unwrap();
        }

        // LED2 toggles every 1000 ms (every 5th 200 ms tick)
        if (tick / 5) % 2 == 0 {
            led2.set_low().unwrap();
        } else {
            led2.set_high().unwrap();
        }

        delay.delay_ms(200u32);
        tick = tick.wrapping_add(1);
    }
}
