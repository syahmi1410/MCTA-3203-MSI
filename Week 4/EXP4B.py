import serial
import time

SERIAL_PORT = 'COM6'  # Replace with your Arduino port
BAUD_RATE = 9600

AUTHORIZED_CARD = "0013020304"

def main():
    arduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset

    print("RFID System Ready. Tap your card...")

    try:
        while True:
            uid = input("Tap card: ").strip()  # Read card UID

            if uid == AUTHORIZED_CARD:
                print(f"[{uid}] - Card Authorized! Turning LED ON...")
                arduino.write(b'1')  # Send ON command
                time.sleep(3)        # Keep LED on
                arduino.write(b'0')  # Turn LED OFF
                print("LED OFF. Ready for next scan.")
            else:
                print(f"[{uid}] - Unauthorized Card! LED remains OFF.")

    except KeyboardInterrupt:
        print("\nExiting program.")
    finally:
        arduino.write(b'0')  # Ensure LED OFF
        arduino.close()
        print("Serial connection closed.")

if __name__ == "__main__":
    main()