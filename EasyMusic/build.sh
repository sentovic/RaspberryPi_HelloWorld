#!/bin/bash

INC_DIRS="-I include -I include/lang -I include/app -I include/message -I include/system"
LIBS="-lpthread -std=c++11"
rm -rf build
mkdir build
mkdir build/obj

# main.cpp
echo [Compiling...]
g++ ${LIBS} ${INC_DIRS} -c src/main.cpp            -o build/obj/main.o
g++ ${LIBS} ${INC_DIRS} -c src/app/application.cpp -o build/obj/app_application.o
g++ ${LIBS} ${INC_DIRS} -c src/system/system.cpp -o build/obj/system_system.o
g++ ${LIBS} ${INC_DIRS} -c src/message/message.cpp -o build/obj/message_message.o
g++ ${LIBS} ${INC_DIRS} -c src/message/looper.cpp  -o build/obj/message_looper.o
g++ ${LIBS} ${INC_DIRS} -c src/message/handler.cpp -o build/obj/message_handler.o

# link
echo [Linking...]
g++ \
    build/obj/main.o \
    build/obj/app_application.o \
    build/obj/message_message.o \
    build/obj/message_looper.o \
    build/obj/message_handler.o \
    build/obj/system_system.o \
    -o build/EasyMusic.exe \
    ${LIBS}

chmod 755 build/EasyMusic.exe

# clean
echo [Cleaning...]
rm -rf build/obj