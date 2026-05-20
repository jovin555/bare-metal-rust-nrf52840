# Threads and Synchronization

Day 8 example: two threads communicate via a message queue.
A producer thread enqueues an incrementing counter every second.
A consumer thread dequeues and logs each value.

## Build
```bash
west build -b nrf52840dk/nrf52840
west flash
```
