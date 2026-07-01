//! Day 7 — GPIOTE: Hardware GPIO Interrupts
//!
//! Replaces the Day 6 polling loop with a true hardware interrupt.
//! BTN1 (P0.11) triggers a GPIOTE interrupt on every falling edge (press).
//! The interrupt handler toggles LED1 and logs the event.
//!
//! Key concepts:
//!   - GPIOTE peripheral routes GPIO edges to the NVIC
//!   - Shared state between main and the ISR uses Mutex<RefCell<Option<T>>>
//!   - cortex_m::interrupt::free() provides a critical section

#![no_std]
#![no_main]

use core::cell::RefCell;

use cortex_m::interrupt::Mutex;
use cortex_m_rt::entry;
use defmt::{info, warn};
use defmt_rtt as _;
use nrf52840_hal::{
    gpio::{p0::Parts, Input, Level, Output, Pin, PullUp, PushPull},
    gpiote::Gpiote,
    pac::{self, interrupt},
    prelude::*,
};
use panic_probe as _;

// Shared state: the LED and GPIOTE handle, accessible from both main and the ISR
static GPIOTE: Mutex<RefCell<Option<Gpiote>>> = Mutex::new(RefCell::new(None));
static LED: Mutex<RefCell<Option<Pin<Output<PushPull>>>>> = Mutex::new(RefCell::new(None));
static LED_STATE: Mutex<RefCell<bool>> = Mutex::new(RefCell::new(false));

#[entry]
fn main() -> ! {
    info!("Day 7: GPIOTE interrupt demo");

    let p = pac::Peripherals::take().unwrap();
    let port0 = Parts::new(p.P0);

    let led1 = port0.p0_13.into_push_pull_output(Level::High).degrade();
    let btn1 = port0.p0_11.into_pullup_input().degrade();

    // Configure GPIOTE channel 0: falling edge on BTN1 → interrupt
    let gpiote = Gpiote::new(p.GPIOTE);
    gpiote
        .channel0()
        .input_pin(&btn1)
        .hi_to_lo()
        .enable_interrupt();

    // Move resources into global static storage before enabling the interrupt
    cortex_m::interrupt::free(|cs| {
        GPIOTE.borrow(cs).replace(Some(gpiote));
        LED.borrow(cs).replace(Some(led1));
    });

    // Enable the GPIOTE interrupt in the NVIC
    unsafe {
        pac::NVIC::unmask(pac::Interrupt::GPIOTE);
    }

    info!("Press BTN1 to toggle LED1");

    loop {
        // Main thread does nothing — all work happens in the ISR
        cortex_m::asm::wfi();
    }
}

// GPIOTE interrupt service routine
#[interrupt]
fn GPIOTE() {
    cortex_m::interrupt::free(|cs| {
        if let Some(gpiote) = GPIOTE.borrow(cs).borrow().as_ref() {
            // Confirm event came from channel 0 and clear it
            if gpiote.channel0().is_event_triggered() {
                gpiote.channel0().reset_events();

                // Toggle LED state
                let mut state = LED_STATE.borrow(cs).borrow_mut();
                *state = !*state;

                if let Some(led) = LED.borrow(cs).borrow_mut().as_mut() {
                    if *state {
                        led.set_low().unwrap(); // LED on
                        info!("BTN1 pressed → LED on");
                    } else {
                        led.set_high().unwrap(); // LED off
                        warn!("BTN1 pressed → LED off");
                    }
                }
            }
        }
    });
}
