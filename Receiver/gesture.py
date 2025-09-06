import serial
import pyautogui
import time

# Replace 'COM3' with your Arduino port
arduino = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  # wait for Arduino to initialize

print("Gesture control started. Move your hand!")

while True:
    try:
        data = arduino.readline().decode().strip()
        if data:
            print("Detected Gesture:", data)
            if data == "NEXT":
                pyautogui.press('right')
            elif data == "PREV":
                pyautogui.press('left')
            elif data == "PLAY_PAUSE":
                pyautogui.press('space')
    except KeyboardInterrupt:
        print("Exiting...")
        break
