#!/bin/sh

case $1 in
    server1-arm)
        cmake -DCMAKE_TOOLCHAIN_FILE=CMakeArm.cmake .
        ;;
    
    server1-avr)
        cmake -DCMAKE_TOOLCHAIN_FILE=CMakeAvr.cmake .
        ;;
    
    server1-amd64)
        cmake .
        ;;
    
    *)
        echo "Unknown build target name $1";
        exit -1;
        ;;
esac
