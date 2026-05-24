# Work Queues and Kernel Timers

Day 23: a 1 Hz timer submits a work item that toggles led0 and logs uptime.
A separate delayable work item prints a status message every 3 s.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```
