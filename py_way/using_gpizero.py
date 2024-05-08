from gpiozero import LED
from time import sleep

led = LED(18)

while True:
    print("ON ")
    led.on()
    sleep(1)
    led.off()
    print("OFF ")
    sleep(1)
