## Contents ##



# Introduction #

Sequanto-automation uses CMake as a compile system, but it should be simple enough to get it to compile using some other build system, some tricks for this is listed at the bottom of this page.

# Configuring #

CMake is designed to separate source files from build files, this means that you will have to create a build directory before you can build sequanto-automation. Normally you just create a sub directory called build, maybe even a sub-directory for each build target inside the build directory.

For instance, when building for windows on windows you would do the following:

```
# mkdir build\msvc-win32
# cd build\msvc-win32
```

Next we need to generate the build files, this is done while being inside the build directory and pointing cmake at the source directory (..\.. in our case):

```
# cmake ..\..
-- Building for: Visual Studio 8 2005
-- Check for working C compiler: cl
-- Check for working C compiler: cl -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: cl
-- Check for working CXX compiler: cl -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Found PythonInterp: C:/Python25/python.exe
-- Looking for stdlib.h
-- Looking for stdlib.h - found
-- Could NOT find Doxygen  (missing:  DOXYGEN_EXECUTABLE)
-- Looking for winsock2.h
-- Looking for winsock2.h - found
-- Configuring done
-- Generating done
-- Build files have been written to: C:/work/sequanto-automation-trunk/build/msvc-win32
```

In most cases this is all that is necessary, on windows this will now have generated the necessary Visual Studio solution and project files on `*`nix it will generate makefiles.

```
# dir /B
ALL_BUILD.vcproj
CMakeCache.txt
CMakeFiles
cmake_install.cmake
generated
libSequantoAutomation.sln
SequantoAutomation.vcproj
server
test_server.vcproj
upload_arduino_demo.bat
ZERO_CHECK.vcproj
```

In some cases you will wish to configure the software before building, this can be done using the cmake-gui command and pointing it to our build directory.

```
cmake-gui .
```

This will let you configure the software:

![http://sequanto-automation.googlecode.com/svn/wiki/images/cmake-gui.png](http://sequanto-automation.googlecode.com/svn/wiki/images/cmake-gui.png)


# Compiling #

Compiling should be as easy as calling `make` on `*`nix or ` devenv <project>.sln ` on win32.

# Cross compiling #

Cross compiling using CMake is done using so called "toolchain" files, these files define the compiler, linker, etc. to use, and the arguments for them.

To cross compile you must tell CMake to use a toolchain file when generating the build files. It is also possible that you mustuse the -G argument to tell cmake which generator to use, for example to generate makefiles when cross compiling for Arduino on windows.

Sequanto-automation ships with two toolchain files, one for AVR (Arduino) and one for ARM, an example call for this could be the following:

```
cmake -DCMAKE_TOOLCHAIN_FILE=..\..\cmake\CMakeAvr.cmake -G "Unix Makefiles" ..\..
```

Detailed explanations for compiling for Arduino on windows is described here: CompilingOnWinAvr.

# Compiling using other build systems #

You are probably not using CMake as the build system for your target, so how do you integrate sequanto-automation into your build system?

It should actually be as simple as just including the source files in server/c/src into your build system, you also need a config.h file for your target, this is most easily done by just using CMake to generate and appropriate one using the above mentioned method.

If you also wish to use the generator (and you do :) you need to call the python script called generator\generate\_automation\_defines.py with your .automation file as input, feel free to use one of the example .automation files as a starting point.