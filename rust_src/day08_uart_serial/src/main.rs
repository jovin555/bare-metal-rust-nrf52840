//! Day 8 — UARTE: Serial Communication
//!
//! Implements a UART echo loop using UARTE0 on the nRF52840 DK.
//! The DK's USB-UART bridge uses:
//!   TX = P0.06   RX = P0.08   (UART_0 on J-Link OB virtual COM port)
//!
//! Connect with any serial terminal at 115200 8N1.
//! Everything you type is echoed back with a "> " prefix.
//!
//! Note: nrf52840-hal UARTE uses DMA — tx/rx buffers must be in RAM (not stack).
//! We use a fixed 64-byte receive buffer here for simplicity.

#![no_std]
#![no_main]

use cortex_m_rt::entry;
use defmt::info;
use defmt_rtt as _;
use nrf52840_hal::{
    gpio::{p0::Parts, Level},
    pac,
    prelude::*,
    uarte::{self, Baudrate, Parity, Uarte},
};
use panic_probe as _;

// Static buffers — UARTE DMA cannot access stack memory
static mut RX_BUF: [u8; 1] = [0u8; 1];
static mut TX_BUF: [u8; 64] = [0u8; 64];

#[entry]
fn main() -> ! {
    info!("Day 8: UARTE serial echo demo");

    let p = pac::Peripherals::take().unwrap();
    let port0 = Parts::new(p.P0);

    // Configure TX and RX pins
    let txd = port0.p0_06.into_push_pull_output(Level::High).degrade();
    let rxd = port0.p0_08.into_floating_input().degrade();

    let mut serial = Uarte::new(
        p.UARTE0,
        uarte::Pins {
            txd,
            rxd,
            cts: None,
            rts: None,
        },
        Parity::EXCLUDED,
        Baudrate::BAUD115200,
    );

    // Send greeting
    let greeting = b"nRF52840 UART echo ready. Type and press Enter.\r\n> ";
    serial.write(greeting).unwrap();

    loop {
        // Read one byte at a time
        let buf = unsafe { &mut RX_BUF };
        serial.read(buf).unwrap();

        let byte = buf[0];
        info!("RX: 0x{:02X} ('{}')", byte, byte as char);

        // Echo the byte back
        let out = unsafe { &mut TX_BUF };
        out[0] = byte;
        serial.write(&out[..1]).unwrap();

        // On newline, print the prompt again
        if byte == b'\r' {
            serial.write(b"\n> ").unwrap();
        }
    }
}
