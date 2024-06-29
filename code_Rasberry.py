import serial
import pyrebase
import RPi.GPIO as GPIO
from time import sleep
import threading

# Thiết lập GPIO cho LED
LED_RED_PIN = 18
LED_GREEN_PIN = 17
LED_YELLOW_PIN = 16 
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(LED_RED_PIN, GPIO.OUT)
GPIO.setup(LED_GREEN_PIN, GPIO.OUT)

# Thiết lập Firebase
config = {     
    "apiKey": "YOUR_API_KEY",
    "authDomain": "YOUR_PROJECT_ID.firebaseapp.com",
    "databaseURL": "https://YOUR_PROJECT_ID-default-rtdb.firebaseio.com/",
    "storageBucket": "YOUR_PROJECT_ID.appspot.com"
}

firebase = pyrebase.initialize_app(config)
database = firebase.database()


def read_serial_data():
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    while True:
        try:
            data = ser.readline().decode().strip()
            values = data.split("\t")

            if len(values) >= 2:
                humid = float(values[0])
                temp = float(values[1])

                print("Humidity:", humid)
                print("Temperature:", temp)
                temperature = database.child("Temperature").set(temp)
                humidity = database.child("Humidity").set(humid)

                GPIO.output(LED_RED_PIN, GPIO.HIGH if temp > 30 else GPIO.LOW)
                GPIO.output(LED_GREEN_PIN, GPIO.HIGH if humid > 50 else GPIO.LOW)

            else:
                print("Data format error:", data)
        except serial.SerialException:
            print("Error reading data from serial port")
            continue  #

serial_thread = threading.Thread(target=read_serial_data)
serial_thread.daemon = True
serial_thread.start()


def control_yellow_led():
    while True:
        try:
            myData = database.child("button").get().val() 
            GPIO.output(LED_YELLOW_PIN, GPIO.HIGH if myData == '1' else GPIO.LOW)
            sleep(1)
        except Exception as e:
            print("Error:", e)

yellow_led_thread = threading.Thread(target=control_yellow_led)
yellow_led_thread.daemon = True
yellow_led_thread.start()

# Giữ chương trình chạy
try:
    while True:
        sleep(1)
except KeyboardInterrupt:
    print("Program terminated by user")
    GPIO.cleanup()
