#ifndef COPI_PNG_H
#define COPI_PNG_H

#include <libpng/png.h>

typedef png_structp PNGStruct;
typedef png_infop   PNGInfo;
typedef png_bytep   PNGBuffer;
typedef int(*ColorConvertor)(int);

enum Config {
    CONFIG_RGBA8888,
    CONFIG_RGB565
};

class PNG {
private:
    PNG();
public:
    ~PNG();

    inline void* getPixels()      { return mBuffer; }
    inline int   getWidth()       { return mWidth; }
    inline int   getHeight()      { return mHeight; }
    inline int   getBitPerPixel() { return mBitPerPixel; }

public:
    static PNG* decodeFile(const char* filePath, enum Config config);
    static int  getConfigBitPerPixel(enum Config config) {
        switch (config) {
        case CONFIG_RGBA8888: return 32;
        case CONFIG_RGB565:   return 16;
        }
    }

private:
    static PNG* decodePNG(PNGStruct png, PNGInfo info, enum Config config);
    static int  convertRGBA8888ToRGB565(int color);
    static int  convertRGB565ToRGBA8888(int color);
    static int  convertNone(int color);
    static ColorConvertor chooseColorConvertor(enum Config configSrc, enum Config configDest);

protected:
    void* allocPixelBuffer(int width, int height, int bitPerPixel);
    void  releasePixelBuffer(void* buffer);

private:
    void* mBuffer;
    int   mChannels;
    int   mWidth;
    int   mHeight;
    int   mBitDepth;
    int   mColorType;
    int   mBitPerPixel;
};

#endif
