mkdir obj
gcc -c ../FrameBuffer/framebuffer.cpp -o ./obj/framebuffer.o -lstdc++
gcc -c png.cpp -o ./obj/png.o -lstdc++
gcc -c sample.cpp -o ./obj/sample.o -lstdc++
gcc ./obj/sample.o ./obj/png.o ./obj/framebuffer.o -o png.exe -lstdc++ -lpng
rm -rf obj
