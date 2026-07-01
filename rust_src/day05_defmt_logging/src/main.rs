//! Day 5 — Logging with defmt and RTT
//!
//! defmt (deferred formatting) sends log messages over RTT (Real-Time Transfer)
//! directly through the debug probe — no UART required. View logs with:
//!
//!   cargo run --release        (probe-rs streams RTT to your terminal)
//!
//! Set log level via the DEFMT_LOG env var (trace/debug/info/warn/error).
//! panic-probe prints a backtrace on panic instead of silently hanging.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use defmt::{debug, error, info, warn};
use defmt_rtt as _;
use embedded_hal::blocking::delay::DelayMs;
use nrf52840_hal::{
    clocks::Clocks,
    delay::Delay,
    gpio::{p0::Parts, Level},
    pac,
};
use panic_probe as _;

#[entry]
fn main() -> ! {
    info!("Day 5: defmt + RTT logging demo");
    info!("Board: nRF52840 DK");

    let p = pac::Peripherals::take().unwrap();
    let core = pac::CorePeripherals::take().unwrap();

    let _clocks = Clocks::new(p.CLOCK).enable_ext_hfosc();
    let mut delay = Delay::new(core.SYST);

    let port0 = Parts::new(p.P0);
    let mut led1 = port0.p0_13.into_push_pull_output(Level::High);

    let mut count = 0u32;
    loop {
        led1.set_low().unwrap();
        info!("LED on  — blink #{}", count);
        delay.delay_ms(500u32);

        led1.set_high().unwrap();
        debug!("LED off — blink #{}", count);
        delay.delay_ms(500u32);

        count = count.wrapping_add(1);

        if count == 5 {
            warn!("Reached 5 blinks — still going");
        }
        if count == 10 {
            error!("Reached 10 blinks — this is a demo error-level log, not a real fault");
        }
    }
}
