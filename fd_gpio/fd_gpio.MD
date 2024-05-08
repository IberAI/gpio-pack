
# fd_gpio: A Python Module for Direct GPIO Control on Raspberry Pi

## Overview

The `fd_gpio` module is a Python extension that facilitates direct control of the GPIO pins on the Raspberry Pi. This module is built in C and utilizes file descriptors and memory mapping for efficient and low-level pin manipulation.

## Key Components

- **fd_gpio.c**: C extension file providing the Python bindings to interact directly with GPIO hardware.
- **setup.py**: Setup script for building and distributing the Python module.
- **test.py**: Script for demonstrating and testing GPIO operations using the `fd_gpio` module.

## How It Works

### Memory Mapping and File Descriptors

1. **Memory Mapping (`mmap`)**: This method is used to map the GPIO physical memory address space into the process's virtual space. This allows the module to access GPIO registers directly from user-space code, which simplifies the control over GPIO pins without needing system calls.

2. **File Descriptors (`open`)**: The module opens `/dev/gpiomem`, which is a character device file providing access to the GPIO memory without requiring root privileges. This approach is secure and adheres to Linux's device handling mechanisms.

### Module Functions

- **`setup_io`**: Initializes the memory mapping for GPIO registers. It opens the GPIO memory device, maps it, and stores the reference for later access.
- **`pin_ready`**: Sets the specified GPIO pin either as an input or an output based on the user's input.
- **`pin_toggle`**: Changes the state of a GPIO pin to high or low, effectively allowing the pin to be turned on or off.

### Python-C Integration

Using the Python C API, the `fd_gpio.c` implements several functions that are exposed to Python. These functions allow Python code to call C functions directly, providing a fast interface for GPIO manipulation. The integration is achieved through defining a series of method mappings that link Python callable names to their C counterparts.

## Why It Works

### Efficiency

The direct interaction with the hardware through memory mapping and file descriptors is much faster than using higher-level abstractions like sysfs or GPIO libraries that rely on file I/O operations. This method reduces the overhead introduced by the system calls and provides a more responsive experience suitable for real-time applications.

### Security and Accessibility

Using `/dev/gpiomem` allows non-root users to interact with GPIO pins, aligning with best practices for device security on Linux systems. It limits the scope of what can be accessed via this file descriptor, focusing solely on GPIO functionalities.

### Ease of Use

By wrapping the GPIO control functionality in a Python module, it provides an accessible interface for developers familiar with Python but not necessarily with C or low-level systems programming. This is particularly beneficial in educational environments or in rapid prototyping scenarios.

## Building and Testing

- The `setup.py` script utilizes Python's Distutils to compile the C extension and package it as a Python module.
- The `test.py` script demonstrates how to use the module to initialize GPIO, set pin modes, and toggle pins.

## Example Usage

```python
import fd_gpio

# Initialize GPIO access
fd_gpio.setup_io()

# Set pin 18 as output and turn it on
fd_gpio.pin_ready(18, False)
fd_gpio.pin_toggle(18, True)
```

