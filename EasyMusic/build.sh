#!/bin/bash

INC_DIRS="-I include -I include/framework -I include/framework/app -I include/framework/message -I include/framework/system -I include/framework/log -I include/framework/json -I include/easymusic -I include/curl"
C_LIBS=""
CXX_LIBS="-lX11 -lpthread -L libs -lcurl -std=c++11"
rm -rf build
mkdir build
mkdir build/obj

# main.cpp
echo [Compiling...]
g++ ${CXX_LIBS} ${INC_DIRS} -c src/main.cpp                      -o build/obj/main.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/framework/app/application.cpp -o build/obj/framework_app_application.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/framework/system/system.cpp   -o build/obj/framework_system_system.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/framework/message/message.cpp -o build/obj/framework_message_message.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/framework/message/looper.cpp  -o build/obj/framework_message_looper.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/framework/message/handler.cpp -o build/obj/framework_message_handler.o
gcc ${C_LIBS}   ${INC_DIRS} -c src/framework/json/cJSON.c        -o build/obj/framework_json_cjson.o
gcc ${C_LIBS}   ${INC_DIRS} -c src/framework/json/cJSON_Utils.c  -o build/obj/framework_json_cjson_utils.o
g++ ${CXX_LIBS} ${INC_DIRS} -c src/easymusic/easymusic.cpp       -o build/obj/easymusic_easymusic.o

# link
echo [Linking...]
g++ \
    build/obj/main.o \
    build/obj/framework_app_application.o \
    build/obj/framework_message_message.o \
    build/obj/framework_message_looper.o \
    build/obj/framework_message_handler.o \
    build/obj/framework_system_system.o \
    build/obj/framework_json_cjson.o \
    build/obj/framework_json_cjson_utils.o \
    build/obj/easymusic_easymusic.o \
    -o build/EasyMusic.exe \
    ${C_LIBS} ${CXX_LIBS}

chmod 755 build/EasyMusic.exe

# clean
echo [Cleaning...]
rm -rf build/obj