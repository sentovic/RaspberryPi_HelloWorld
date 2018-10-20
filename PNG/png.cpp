#include "png.h"
#include <stdio.h>
#include <string.h>
#include <linux/types.h> // u8, u16, u32

#define DEBUG     true
#define RGB_565   0
#define RGBA_8888 1
#define RED       0
#define GREEN     1
#define BLUE      2
#define ALPHA     3

typedef char  u8;
typedef __u16 u16;
typedef __u32 u32;

static unsigned int COLOR_MASK[][4] = {
   {0xF8, 0xFC, 0xF8},       // RGB_565
   {0xFF, 0xFF, 0xFF, 0xFF}, // RGBA_8888
};

#define RGB565(r, g, b)       ( ((r & COLOR_MASK[RGB_565][RED])   << 11) \
                              | ((g & COLOR_MASK[RGB_565][GREEN]) << 5)  \
                              | ((b & COLOR_MASK[RGB_565][BLUE])  << 0))
#define RGBA8888(r, g, b, a)  ( ((r & COLOR_MASK[RGBA_8888][RED])   << 24) \
                              | ((g & COLOR_MASK[RGBA_8888][GREEN]) << 16) \
                              | ((b & COLOR_MASK[RGBA_8888][BLUE])  << 8)  \
                              | ((a & COLOR_MASK[RGBA_8888][ALPHA]) << 0))
#define RGBA8888_RED(color)   ((color >> 24) & COLOR_MASK[RGBA_8888][RED])
#define RGBA8888_GREEN(color) ((color >> 16) & COLOR_MASK[RGBA_8888][GREEN])
#define RGBA8888_BLUE(color)  ((color >> 8)  & COLOR_MASK[RGBA_8888][BLUE])
#define RGBA8888_ALPHA(color) ((color >> 0)  & COLOR_MASK[RGBA_8888][ALPHA])
#define RGB565_RED(color)     ((color >> 11) & COLOR_MASK[RGB_565][RED])
#define RGB565_GREEN(color)   ((color >> 5)  & COLOR_MASK[RGB_565][GREEN])
#define RGB565_BLUE(color)    ((color >> 0)  & COLOR_MASK[RGB_565][BLUE])

PNG::PNG() {
    mBuffer      = NULL;
    mChannels    = 0;
    mWidth       = 0;
    mHeight      = 0;
    mBitDepth    = 0;
    mColorType   = 0;
    mBitPerPixel = 0;
}

PNG::~PNG() {
    if (mBuffer != NULL) {
        releasePixelBuffer(mBuffer);
        mBuffer = NULL;
    }
}

void* PNG::allocPixelBuffer(int width, int height, int bitPerPixel) {
    int byteSize = width * height * bitPerPixel / 8;
    if (byteSize > 0) {
        return new char[byteSize];
    } else {
        return NULL;
    }
}

void PNG::releasePixelBuffer(void* buffer) {
    if (buffer != NULL) {
        delete[] (char*)buffer;
    }
}

PNG* PNG::decodeFile(const char* filePath, enum Config config) {
    if ((filePath == NULL) || (strlen(filePath) <= 0)) {
        printf("[DecodeFile] file path is empty\n");
        return NULL;
    }
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) {
        printf("[DecodeFile] file %s cannot open\n", filePath);
        return NULL;
    }

    PNGStruct png     = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    PNGInfo   pngInfo = png_create_info_struct(png);

    setjmp(png_jmpbuf(png));
    rewind(file);
    png_init_io(png, file);

    printf("[DecodePNG] decoding PNG from file %s\n", filePath);
    PNG* pngImage = decodePNG(png, pngInfo, config);
    if (pngImage == NULL) {
        printf("[DecodePNG] decode PNG from file %s, failed\n", filePath);
    }

    fclose(file);
    png_destroy_read_struct(&png, &pngInfo, 0);
    return pngImage;
}

PNG* PNG::decodePNG(PNGStruct png, PNGInfo info, enum Config config) {
    png_read_png(png, info, PNG_TRANSFORM_EXPAND, 0);

    int        channels     = png_get_channels(png, info);
    int        width        = png_get_image_width(png, info);
    int        height       = png_get_image_height(png, info);
    int        bitDepth     = png_get_bit_depth(png, info);
    int        colorType    = png_get_color_type(png, info);
    int        configBits   = getConfigBitPerPixel(config);
    int        configBytes  = configBits / 8;
    int        bytePerLine  = bitDepth * channels * width;
    int        bytePerPixel = bitDepth * channels;
    PNGBuffer* buffer       = png_get_rows(png, info);

    PNG* pngImage = new PNG();
    pngImage->mChannels    = CONFIG_RGB565 ? 3 : (CONFIG_RGBA8888 ? 4 : channels);
    pngImage->mWidth       = width;
    pngImage->mHeight      = height;
    pngImage->mBitDepth    = bitDepth;
    pngImage->mBitPerPixel = CONFIG_RGB565 ? 16 : (CONFIG_RGBA8888 ? 32 : channels * bitDepth);
    pngImage->mColorType   = colorType;
    pngImage->mBuffer      = pngImage->allocPixelBuffer(width, height, configBits);

    switch (colorType) {
        case PNG_COLOR_TYPE_RGB_ALPHA: {
            for (int h = 0; h < height; h++) {
                for (int w = 0; w < width; w++) {
                    int index = h * width + w;
                    if (config == CONFIG_RGBA8888) {
                        u32* pixelBuffer = (u32*) pngImage->mBuffer;
                        pixelBuffer[index] = RGBA8888(
                                               buffer[h][w * 4 + RED],
                                               buffer[h][w * 4 + GREEN],
                                               buffer[h][w * 4 + BLUE],
                                               buffer[h][w * 4 + ALPHA]);
                    } else if (config == CONFIG_RGB565) {
                        u16* pixelBuffer = (u16*) pngImage->mBuffer;
                        pixelBuffer[index] = RGB565(
                                               buffer[h][w * 4 + RED],
                                               buffer[h][w * 4 + GREEN],
                                               buffer[h][w * 4 + BLUE]);
                    }
                }
            }
        } break;

        case PNG_COLOR_TYPE_RGB: {
            for (int h = 0; h < height; h++) {
                for (int w = 0; w < width; w++) {
                    int index = h * width + w;
                    if (config == CONFIG_RGBA8888) {
                        u32* pixelBuffer = (u32*) pngImage->mBuffer;
                        pixelBuffer[index] = RGBA8888(
                                               buffer[h][w * 3 + RED],
                                               buffer[h][w * 3 + GREEN],
                                               buffer[h][w * 3 + BLUE],
                                               0xFF);
                    } else if (config == CONFIG_RGB565) {
                        u16* pixelBuffer = (u16*) pngImage->mBuffer;
                        pixelBuffer[index] = RGB565(
                                               buffer[h][w * 3 + RED],
                                               buffer[h][w * 3 + GREEN],
                                               buffer[h][w * 3 + BLUE]);
                    }
                }
            }
        } break;
    }

#if DEBUG
    printf("[DecodePNG]   - size is (%d, %d)\n", width, height);
    printf("[DecodePNG]   - bit depth is %dbits\n", bitDepth);
    printf("[DecodePNG]   - color type is %d\n", colorType);
    printf("[DecodePNG]   - channels has %d\n", channels);
#endif

    return pngImage;
}

int PNG::convertRGBA8888ToRGB565(int color) {
    int red   = RGBA8888_RED(color);
    int green = RGBA8888_GREEN(color);
    int blue  = RGBA8888_BLUE(color);
    return RGB565(red, green, blue);
}

int PNG::convertRGB565ToRGBA8888(int color) {
    int red   = RGB565_RED(color);
    int green = RGB565_GREEN(color);
    int blue  = RGB565_BLUE(color);
    return RGBA8888(red, green, blue, 0xFF);
}

int PNG::convertNone(int color) {
    return color;
}

ColorConvertor PNG::chooseColorConvertor(enum Config configSrc, enum Config configDest) {
    if (configSrc == configDest) {
         return NULL;
    } else if ((configSrc == CONFIG_RGBA8888) && (configDest == CONFIG_RGB565)) {
         return convertRGBA8888ToRGB565;
    } else if ((configSrc == CONFIG_RGB565) && (configDest == CONFIG_RGBA8888)) {
         return convertRGB565ToRGBA8888;
    } else {
         return NULL;
    }
}



