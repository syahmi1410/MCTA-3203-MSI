import serial
import time
from collections import deque
import numpy as np

# -- Replace 'COMx' with your Arduino's serial port
SERIAL_PORT = 'COM6'
BAUD_RATE = 9600

# Buffers to store the last 40 data points (approx. 2 seconds of motion)
history_x = deque(maxlen=40)
history_y = deque(maxlen=40)


def is_circular_motion(x_data, y_data):
    """
    Analyzes X and Y accelerometer data.
    Returns True if the movement represents a circular motion.
    """
    if len(x_data) < 10:
        return False

    x = np.array(x_data)
    y = np.array(y_data)

    # Step 1: Center the data
    x_centered = x - np.mean(x)
    y_centered = y - np.mean(y)

    # Step 2: Amplitude check (ignore tiny jitters)
    if np.std(x_centered) < 0.5 or np.std(y_centered) < 0.5:
        return False

    # Step 3: Circularity check (std ratio close to 1)
    ratio = np.std(x_centered) / np.std(y_centered)
    if ratio < 0.7 or ratio > 1.3:
        return False

    # Step 4: Continuous rotation check using cross product
    cross_products = []
    for i in range(len(x_centered) - 1):
        x1, y1 = x_centered[i], y_centered[i]
        x2, y2 = x_centered[i + 1], y_centered[i + 1]
        cross = x1 * y2 - y1 * x2
        cross_products.append(cross)

    cross_products = np.array(cross_products)
    clockwise_ratio = np.sum(cross_products > 0) / len(cross_products)
    counter_ratio = np.sum(cross_products < 0) / len(cross_products)

    # Step 5: Threshold for circle detection
    if clockwise_ratio > 0.7 or counter_ratio > 0.7:
        return True

    return False


def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset
    print("System ready. Move the MPU6050 in a circle to toggle the LED...")

    led_is_on = False

    try:
        while True:
            raw = ser.readline().decode('utf-8', errors='ignore').strip()
            if raw:
                parts = raw.split(',')

                if len(parts) >= 2:
                    try:
                        ax = float(parts[0])
                        ay = float(parts[1])

                        history_x.append(ax)
                        history_y.append(ay)

                        if len(history_x) == 40:
                            if is_circular_motion(history_x, history_y):
                                led_is_on = not led_is_on
                                if led_is_on:
                                    print("Circle Detected! Toggling LED: ON")
                                    ser.write(b'O')
                                else:
                                    print("Circle Detected! Toggling LED: OFF")
                                    ser.write(b'F')

                                history_x.clear()
                                history_y.clear()

                    except ValueError:
                        continue

    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    finally:
        ser.write(b'F')
        ser.close()
        print("Serial connection closed.")


if __name__ == "__main__":
    main()