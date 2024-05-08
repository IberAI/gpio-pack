import fd_gpio
import time

def test_gpio_operations():
    try:
        # Initialize GPIO
        print("Initializing GPIO...")
        fd_gpio.setup_io()

        # Test GPIO pin 18
        pin = 18

        # Set pin as output
        print("Setting pin {} as output...".format(pin))
        fd_gpio.pin_ready(pin, False)

        # Toggle pin high
        print("Setting pin {} high...".format(pin))
        fd_gpio.pin_toggle(pin, True)
        time.sleep(4)  # Wait for 1 second

        # Toggle pin low
        print("Setting pin {} low...".format(pin))
        fd_gpio.pin_toggle(pin, False)
        time.sleep(1)  # Wait for 1 second

        print("GPIO operations completed successfully.")

    except Exception as e:
        print("An error occurred: {}".format(e))

if __name__ == "__main__":
    test_gpio_operations()

