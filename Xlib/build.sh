#!/bin/bash

INC_DIRS="-I src"
C_LIBS=""
CXX_LIBS="-lX11 -lpthread -L libs -lcurl -std=c++11"
rm -rf build
mkdir build
mkdir build/obj

# main.cpp
echo [Compiling...]
g++ ${CXX_LIBS} ${INC_DIRS} -c src/main.cpp                      -o build/obj/main.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/basic.cpp                     -o build/obj/basic.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/xrenderer.cpp                 -o build/obj/xrenderer.o

# link
echo [Linking...]
g++ \
    build/obj/main.o \
    build/obj/basic.o \
    build/obj/xrenderer.o \
    -o build/XDemo.exe \
    ${C_LIBS} ${CXX_LIBS}

chmod 755 build/XDemo.exe

# clean
echo [Cleaning...]
rm -rf build/obj