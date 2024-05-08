#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#define GPIO_BASE 0x3F200000
#define BLOCK_SIZE (4*1024)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |= (1<<(((g)%10)*3))
#define GPIO_SET *(gpio+7)
#define GPIO_CLR *(gpio+10)

volatile unsigned *gpio;

static PyObject *setup_io(PyObject *self, PyObject *args) {
    int mem_fd;
    void *gpio_map;

    if ((mem_fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) < 0) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to open /dev/gpiomem");
        return NULL;
    }

    gpio_map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to map GPIO memory");
        return NULL;
    }

    gpio = (volatile unsigned *)gpio_map;
    printf("GPIO memory initialized.\n");
    Py_RETURN_NONE;
}

static PyObject *pin_ready(PyObject *self, PyObject *args) {
    int gpio_pin;
    int pin_mode;
    if (!PyArg_ParseTuple(args, "ii", &gpio_pin, &pin_mode)) {
        return NULL;
    }

    printf("pin_ready called with gpio_pin=%d, pin_mode=%d\n", gpio_pin, pin_mode);

    if (gpio_pin < 0 || gpio_pin > 27) {
        PyErr_SetString(PyExc_ValueError, "GPIO pin is out of valid range (0-27)");
        return NULL;
    }

    INP_GPIO(gpio_pin);
    if (!pin_mode) {
        OUT_GPIO(gpio_pin);
    }
    
    printf("GPIO pin %d set to %d mode.\n", gpio_pin, pin_mode);
    Py_RETURN_NONE;
}

static PyObject *pin_toggle(PyObject *self, PyObject *args) {
    int gpio_pin;
    int pin_state;
    if (!PyArg_ParseTuple(args, "ii", &gpio_pin, &pin_state)) {
        return NULL;
    }

    printf("pin_toggle called with gpio_pin=%d, pin_state=%d\n", gpio_pin, pin_state);

    if (pin_state) {
        GPIO_SET = 1 << gpio_pin;
    } else {
        GPIO_CLR = 1 << gpio_pin;
    }

    printf("GPIO pin %d toggled to %d.\n", gpio_pin, pin_state);
    return PyBool_FromLong(pin_state);
}

static PyMethodDef FdGpioMethods[] = {
    {"setup_io",  setup_io, METH_NOARGS, "Initialize GPIO memory mapping"},
    {"pin_ready", pin_ready, METH_VARARGS, "Prepare GPIO pin as input or output"},
    {"pin_toggle", pin_toggle, METH_VARARGS, "Toggle GPIO pin on or off"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef fd_gpio_module = {
    PyModuleDef_HEAD_INIT,
    "fd_gpio",   /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module,
                     or -1 if the module keeps state in global variables. */
    FdGpioMethods
};

PyMODINIT_FUNC PyInit_fd_gpio(void) {
    return PyModule_Create(&fd_gpio_module);
}

