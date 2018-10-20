mkdir obj
gcc -c framebuffer.cpp -o ./obj/framebuffer.o -lstdc++
gcc -c sample.cpp -o ./obj/sample.o -lstdc++
gcc ./obj/sample.o ./obj/framebuffer.o -lstdc++ -o framebuffer.exe -lstdc++
rm -rf ./obj
