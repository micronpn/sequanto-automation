## Prerequisites ##

  * CMake version 2.8.0 (or later)
> > http://cmake.org/

  * Install WinAVR 20100110 (or later) remember to let the installer modify your PATH.
> > http://winavr.sourceforge.net/

## Configuring ##

  * Checkout sequanto-automation/trunk somewhere on your harddrive.

  * Create a build directory for arduino/avr, e.g. build\avr\

  * Open a terminal in the created build directory.

  * Invoke cmake:


> ` cmake -DCMAKE_TOOLCHAIN_FILE=..\..\cmake\CmakeAvr.cmake -G "Unix Makefiles" ..\.. `

> You must invoke one more time to get things to be defined correctly (not sure why!), now the build directory knows where it was generated from so you can just write:

> ` cmake . `

  * After this step it is easier to just invoke the cmake gui command to configure the source, although everything should be configured correctly by now.

> ` cmake-gui `


## Building ##

Building should be as simple as calling ` make ` in the build directory.

## Uploading to the arduino ##

The two test applications that comes with sequanto-automation can be uploaded to the arduino (after they are built) by using two batch scripts that is also generated as part of the build process.

  * ` upload_arduino_test.bat ` uploads the arduino\_test application
  * ` upload_test_server.bat ` uploads the test\_server application