# Multiplatform OpenGL GameEngine

## Prequisites

You need to have python installed, and PYTHON_EXECUTABLE set for this to work.
And of course you need to have cmake, and a c/c++ compiler. For Windows I highly recommend to use Visual Studio build tools. For MacOS and Linux I recommend gnu make to compile it (Not Xcode! It will be very messy and probably won't work, unless you really know what you do).

## Build

Compile the project with:

```bash
./build.sh
```

It will generate a platform-specific build system for your project.

run
```bash
./run.sh
```
or directly:
```bash
cd build
./PixelEngine
```
NOTE: You need to be in the build directory or else it won't find the shader files.

It can be more complicated on winodws for example.
I haven't tried my build script on windows - it maybe even works, try it out.
Else just compile it by hand as noted in #Prequisites.

Have fun with it!
