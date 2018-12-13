# RaspberryPi_HelloWorld
HelloWorld project set in raspberry pi for practice

-----
# RaspberryPi_HelloWorld
树莓派 HelloWorld 练习工程集

### 1 Gpio
通过```/sys/class/gpio/```操作GPIO
提供的函数：
 - ```bool openGpio(int port);```
 - ```bool setupGpioDirection(int port, int direction);```
 - ```bool setupGpioValue(int port, int value);```
 - ```bool setupGpioEdge(int port, int edge);```
 - ```int  readGpioValue(int port);```
 - ```bool closeGpio(int port);```
 
### 2 RotaryEncoder
通过linux的poll机制，获取Gpio事件，实现旋转编码器
提供的函数：
 - ```RotaryEncoder::RotaryEncoder(int portCLK, int portDAT);```
 - ```RotaryEncoder::RotaryEncoder(int portCLK, int portDAT, int portBTN);```
 - ```RotaryEncoder::~RotaryEncoder();```
 - ```void RotaryEncoder::enable();```
 - ```bool RotaryEncoder::openGpios();```
 - ```bool RotaryEncoder::closeGpios();```
 - ```int RotaryEncoder::getPinFD(int index);```
 - ```ThreadWrapper* RotaryEncoder::getListenThread();```
 
### 3 PNG
使用libpng解码PNG图片
提供的函数：
 - ```PNG* PNG::decodeFile(const char* filePath, enum Config config);```
 - ```PNG::~PNG();```
 - ```void* PNG::getPixels();```
 - ```int PNG::getWidth();```
 - ```int PNG::getHeight();```
 - ```int PNG::getBitPerPixel();```
 - ```int  getConfigBitPerPixel(enum Config config);```
 
### 4 FrameBuffer
读写linux FrameBuffer
提供的函数：
 - ```int enumerateFrameBufferDevice(const char** buffer);```
 - ```FrameBuffer::FrameBuffer(const char* deviceNode);```
 - ```FrameBuffer::~FrameBuffer();```
 - ```int FrameBuffer::getWidth();```
 - ```int FrameBuffer::getHeight();```
 - ```int FrameBuffer::getBitPerPixel();```
 - ```void FrameBuffer::cleanFramebuffer();```
 - ```void FrameBuffer::fillColor(unsigned int color);```
 - ```void FrameBuffer::drawImage(void* pixels, int width, int height, int bitPerPixel);```
