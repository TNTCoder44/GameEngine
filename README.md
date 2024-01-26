# Multiplatform OpenGL GameEngine

## prequisites

You need to have python installed, and PYTHON_EXECUTABLE set for this to work.
And of course you need to have cmake, and a c/c++ compiler. For Windows I highly recommend to use Visual Studio build tools. For MacOS and Linux I highly recommend gnu make to compile it(Not Xcode! It will be very messy!).

## Build

Compile the project with:

```bash
mkdir build && cd build
cmake ..
```

It will generate a platform-specific build system for your project.

To compile it for example with linux makefiles:

```bash
cd build
make
```

It can be more complicated on winodws for example.
