//! Day 10 — RTIC: Real-Time Interrupt-Driven Concurrency
//!
//! RTIC (Real-Time Interrupt-driven Concurrency) is a framework that turns
//! Rust interrupt handlers into first-class tasks with statically-verified
//! resource sharing — no Mutex<RefCell<Option<T>>> boilerplate from Day 7.
//!
//! This example:
//!   - BTN1 press → GPIOTE task → toggles LED1 (hardware interrupt priority 1)
//!   - LED2 blinks in the idle task at a fixed pace
//!   - Both tasks share a `press_count` counter safely

#![no_std]
#![no_main]

use defmt_rtt as _;
use panic_probe as _;

#[rtic::app(device = nrf52840_hal::pac, peripherals = true, dispatchers = [SWI0_EGU0])]
mod app {
    use defmt::info;
    use nrf52840_hal::{
        gpio::{p0::Parts, Input, Level, Output, Pin, PullUp, PushPull},
        gpiote::Gpiote,
        pac,
        prelude::*,
    };

    #[shared]
    struct Shared {
        press_count: u32,
    }

    #[local]
    struct Local {
        gpiote: Gpiote,
        led1: Pin<Output<PushPull>>,
        led1_state: bool,
        led2: Pin<Output<PushPull>>,
        _btn1: Pin<Input<PullUp>>,
    }

    #[init]
    fn init(cx: init::Context) -> (Shared, Local, init::Monotonics) {
        info!("Day 10: RTIC concurrency demo");

        let p = cx.device;
        let port0 = Parts::new(p.P0);

        let led1 = port0.p0_13.into_push_pull_output(Level::High).degrade();
        let led2 = port0.p0_14.into_push_pull_output(Level::High).degrade();
        let btn1 = port0.p0_11.into_pullup_input().degrade();

        // Wire BTN1 falling edge to GPIOTE channel 0
        let gpiote = Gpiote::new(p.GPIOTE);
        gpiote
            .channel0()
            .input_pin(&btn1)
            .hi_to_lo()
            .enable_interrupt();

        (
            Shared { press_count: 0 },
            Local {
                gpiote,
                led1,
                led1_state: false,
                led2,
                _btn1: btn1,
            },
            init::Monotonics(),
        )
    }

    // Runs when there is no higher-priority task ready
    #[idle(local = [led2])]
    fn idle(cx: idle::Context) -> ! {
        loop {
            cx.local.led2.set_low().unwrap();
            cortex_m::asm::delay(4_000_000);
            cx.local.led2.set_high().unwrap();
            cortex_m::asm::delay(4_000_000);
        }
    }

    // Hardware task bound to the GPIOTE interrupt
    #[task(binds = GPIOTE, local = [gpiote, led1, led1_state], shared = [press_count])]
    fn on_button(mut cx: on_button::Context) {
        cx.local.gpiote.channel0().reset_events();

        *cx.local.led1_state = !*cx.local.led1_state;
        if *cx.local.led1_state {
            cx.local.led1.set_low().unwrap();
        } else {
            cx.local.led1.set_high().unwrap();
        }

        cx.shared.press_count.lock(|count| {
            *count += 1;
            info!("BTN1 pressed — total presses: {}", *count);
        });
    }
}
