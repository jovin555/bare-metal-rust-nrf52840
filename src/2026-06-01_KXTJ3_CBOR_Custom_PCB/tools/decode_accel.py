#!/usr/bin/env python3
"""
KXTJ3-1057 CBOR decoder
Receives CBOR-encoded accelerometer frames from the nRF52840 over USB CDC-ACM
and prints a live table. Optional --plot flag shows a scrolling graph.

Usage:
    python3 decode_accel.py --port /dev/ttyACM0
    python3 decode_accel.py --port /dev/ttyACM0 --plot
    python3 decode_accel.py --port COM5 --plot          # Windows

Dependencies:
    pip install pyserial cbor2 matplotlib
"""

import argparse
import sys
import serial
import cbor2


def parse_args():
    p = argparse.ArgumentParser(description="Decode KXTJ3 CBOR stream over USB")
    p.add_argument("--port", required=True, help="Serial port (e.g. /dev/ttyACM0 or COM5)")
    p.add_argument("--baud", type=int, default=115200, help="Baud rate (default: 115200)")
    p.add_argument("--plot", action="store_true", help="Show live scrolling plot")
    p.add_argument("--count", type=int, default=0,
                   help="Stop after N frames (0 = run forever)")
    return p.parse_args()


def print_header():
    print(f"{'SEQ':>6}  {'X (mg)':>8}  {'Y (mg)':>8}  {'Z (mg)':>8}  {'Range':>6}")
    print(f"{'----':>6}  {'--------':>8}  {'--------':>8}  {'--------':>8}  {'-----':>6}")


def print_frame(frame: dict):
    seq  = frame.get("seq", "?")
    x    = frame.get("x",   0)
    y    = frame.get("y",   0)
    z    = frame.get("z",   0)
    rng  = frame.get("rng", "?")
    print(f"{seq:>6}  {x:>8}  {y:>8}  {z:>8}  ±{rng}g")


def run_plain(port: str, baud: int, max_frames: int):
    print(f"Connecting to {port} at {baud} baud...")
    with serial.Serial(port, baud, timeout=2) as ser:
        print("Connected. Waiting for CBOR frames (send 'accel cbor' or 'accel stream N').\n")
        print_header()
        received = 0
        while True:
            line = ser.readline()
            if not line:
                continue
            line = line.strip()
            if not line:
                continue
            try:
                frame = cbor2.loads(line)
                if isinstance(frame, dict):
                    print_frame(frame)
                    received += 1
                    if max_frames and received >= max_frames:
                        print(f"\nReceived {received} frames. Done.")
                        break
            except Exception as e:
                # Non-CBOR line (shell output, log messages) — print as text
                try:
                    print(f"[text] {line.decode('utf-8', errors='replace')}")
                except Exception:
                    pass


def run_plot(port: str, baud: int, max_frames: int):
    try:
        import matplotlib.pyplot as plt
        import matplotlib.animation as animation
        from collections import deque
    except ImportError:
        print("matplotlib not installed. Run: pip install matplotlib")
        sys.exit(1)

    WINDOW = 100  # number of samples to display
    xs = deque([0] * WINDOW, maxlen=WINDOW)
    ys = deque([0] * WINDOW, maxlen=WINDOW)
    zs = deque([0] * WINDOW, maxlen=WINDOW)
    seqs = deque([0] * WINDOW, maxlen=WINDOW)

    ser = serial.Serial(port, baud, timeout=0.1)
    print(f"Connected to {port}. Close the plot window to stop.\n")

    fig, ax = plt.subplots(figsize=(12, 5))
    line_x, = ax.plot([], [], label="X (mg)", color="red")
    line_y, = ax.plot([], [], label="Y (mg)", color="green")
    line_z, = ax.plot([], [], label="Z (mg)", color="blue")
    ax.set_ylim(-2000, 2000)
    ax.set_xlim(0, WINDOW)
    ax.set_xlabel("Sample")
    ax.set_ylabel("Acceleration (mg)")
    ax.set_title("KXTJ3-1057 Live Accelerometer Data")
    ax.legend(loc="upper right")
    ax.axhline(0, color="gray", linewidth=0.5)
    ax.grid(True, alpha=0.3)

    received = [0]

    def update(_frame):
        # Drain all pending lines from serial buffer
        while ser.in_waiting:
            line = ser.readline().strip()
            if not line:
                continue
            try:
                data = cbor2.loads(line)
                if isinstance(data, dict):
                    xs.append(data.get("x", 0))
                    ys.append(data.get("y", 0))
                    zs.append(data.get("z", 0))
                    seqs.append(data.get("seq", 0))
                    received[0] += 1
                    ax.set_title(
                        f"KXTJ3-1057 Live Data  |  seq={data.get('seq','?')}  "
                        f"range=±{data.get('rng','?')}g"
                    )
                    if max_frames and received[0] >= max_frames:
                        ani.event_source.stop()
                        ser.close()
            except Exception:
                pass

        idx = list(range(WINDOW))
        line_x.set_data(idx, list(xs))
        line_y.set_data(idx, list(ys))
        line_z.set_data(idx, list(zs))
        return line_x, line_y, line_z

    ani = animation.FuncAnimation(fig, update, interval=50, blit=True)
    plt.tight_layout()
    plt.show()
    if ser.is_open:
        ser.close()


def main():
    args = parse_args()
    try:
        if args.plot:
            run_plot(args.port, args.baud, args.count)
        else:
            run_plain(args.port, args.baud, args.count)
    except serial.SerialException as e:
        print(f"Serial error: {e}")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nStopped.")


if __name__ == "__main__":
    main()
