# USB CDC-ACM Communication

Day 11 example: the nRF52840 enumerates as a USB virtual serial port.
After connecting, open a terminal on your PC and observe the counter output.
Any byte you send back is echoed with a prefix.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Connect
- Linux:   `minicom -D /dev/ttyACM0`
- macOS:   `screen /dev/tty.usbmodem* 115200`
- Windows: open the COMx port in PuTTY
