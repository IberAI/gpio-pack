from distutils.core import setup, Extension

# Define the extension module for the GPIO functions, specifying the source file.
module1 = Extension('fd_gpio',
                    sources = ['fd_gpio.c'])

# Use the setup function to prepare the package for distribution.
setup(name='FdGPIO',
      version='1.0',
      description='This is a GPIO package for Raspberry Pi that uses file descriptors for direct memory access to control GPIO pins.',
      long_description="""This Python module provides direct control over GPIO pins on the Raspberry Pi via memory mapping,
                          making it suitable for high-performance applications where low-level pin control is necessary.
                          Users can initialize GPIO memory, set pins as input or output, and toggle pin states.""",
      author='ilteber "Iber" Dover',
      author_email='ilteber.dover@gmail.com',
      url='https://github.com/IberAI/gpio-pack',  # Optional: include a URL where users can find more information about this module
      classifiers=[
          'Development Status :: 5 - Production/Stable',
          'Intended Audience :: Developers',
          'Topic :: Software Development :: Libraries :: Python Modules',
          'License :: MIT License',  # Optional: Choose an appropriate license
          'Programming Language :: Python :: 3',
          'Programming Language :: Python :: 3.6',
          'Programming Language :: Python :: 3.7',
          'Programming Language :: Python :: 3.8',
          'Programming Language :: Python :: 3.9',
          'Operating System :: POSIX :: Linux'
      ],
      ext_modules=[module1])

