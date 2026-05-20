# CBOR Encoding over USB

Day 12 example: encodes a sensor data map `{"temp": <val>, "hum": <val>, "seq": <n>}`
using zcbor and sends the binary frame over USB CDC-ACM every second.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Decode on PC (Python)
```bash
pip install pyserial cbor2
python3 -c "
import serial, cbor2, time
s = serial.Serial('/dev/ttyACM0', timeout=2)
while True:
    ln = s.readline()
    if ln:
        print(cbor2.loads(ln.strip()))
"
```
