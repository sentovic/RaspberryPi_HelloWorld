#!/bin/bash

mkdir obj
gcc -c ../Gpio/gpio.c -o ./obj/gpio.o
gcc -c rotaryencoder.cpp -o ./obj/rotaryencoder.o -lstdc++ -lpthread
gcc ./obj/rotaryencoder.o ./obj/gpio.o -lstdc++ -lpthread -o rotaryencoder.exe
rm -rf ./obj
