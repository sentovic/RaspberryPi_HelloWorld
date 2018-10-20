#include "framebuffer.h"
#include <string.h>      // strlen(), strcpy()
#include <stdio.h>       // printf()
#include <fcntl.h>       // O_RDWR, open()
#include <unistd.h>      // close(), sleep()
#include <sys/ioctl.h>   // ioctl()
#include <sys/mman.h>    // mmap(), munmap()
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

int FrameBuffer::enumerateFrameBufferDevice(const char** list) {
    if (list != NULL) {
        // TODO
    }
    return 0;
}

FrameBuffer::FrameBuffer(const char* deviceNode) {
    int nameLength = strlen(deviceNode);
    if (nameLength > 0) {
        mDeviceNode = new char[nameLength + 1];
        strcpy(mDeviceNode, deviceNode);
    }

    openFramebuffer();
}

FrameBuffer::~FrameBuffer() {
    if (mDeviceNode != NULL) {
        delete [] mDeviceNode;
        mDeviceNode = NULL;
    }

    closeFramebuffer();
}

bool FrameBuffer::openFramebuffer() {
    if (mDeviceNode == NULL) {
        return false;
    }

    int fd = open(mDeviceNode, O_RDWR);
    if (fd < 0) {
        printf("[Open] cannot open framebuffer %s\n", mDeviceNode);
        return false;
    }

    memset(&mFramebufferFixInfo, 0, sizeof(FixInfo));
    if (ioctl(fd, FBIOGET_FSCREENINFO, &mFramebufferFixInfo)) {
        printf("[Open] cannot read fixed information of Framebuffer %s\n", mDeviceNode);
        closeFramebuffer();
        return false;
    } else {
#if DEBUG
        printf("[Open] Framebuffer %s\n", mDeviceNode);
        printf("[Open]   - id is %s\n", mFramebufferFixInfo.id);
        printf("[Open]   - physical address is 0x%ld\n", mFramebufferFixInfo.smem_start);
        printf("[Open]   - physical address length is %d\n", mFramebufferFixInfo.smem_len);
        printf("[Open]   - mapped IO address is 0x%ld\n", mFramebufferFixInfo.mmio_start);
        printf("[Open]   - mapped IO address length is %d\n", mFramebufferFixInfo.mmio_len);
        printf("[Open]   - line bytes size is %dbytes\n", mFramebufferFixInfo.line_length);
#endif
    }

    memset(&mFramebufferVarInfo, 0, sizeof(VarInfo));
    if (ioctl(fd, FBIOGET_VSCREENINFO, &mFramebufferVarInfo)) {
        printf("[Open] cannot read variable information of Framebuffer %s\n", mDeviceNode);
        closeFramebuffer();
        return false;
    } else {
#if DEBUG
        printf("[Open]   - physical size is (%dmm, %dmm)\n", mFramebufferVarInfo.width, mFramebufferVarInfo.height);
        printf("[Open]   - resolution is (%d, %d)\n", mFramebufferVarInfo.xres, mFramebufferVarInfo.yres);
        printf("[Open]   - bits per pixel is %dbits\n", mFramebufferVarInfo.bits_per_pixel);
        printf("[Open]   - %s standard pixel format\n", (mFramebufferVarInfo.nonstd != 0) ? "isn't" : "is");
        printf("[Open]   - gray scale is %d\n", mFramebufferVarInfo.grayscale);
        printf("[Open]   - rotation is %d\n", mFramebufferVarInfo.rotate);
        printf("[Open]   - padding is (%d, %d - %d, %d)\n",
               mFramebufferVarInfo.left_margin, mFramebufferVarInfo.upper_margin,
               mFramebufferVarInfo.right_margin, mFramebufferVarInfo.lower_margin);
#endif
    }

    int framebufferSize = getWidth() * getHeight() * getBitPerPixel() / 8;
    mFramebuffer = (Byte*)mmap(NULL, framebufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mFramebuffer == NULL) {
        printf("[Open] cannot map to framebuffer %s", mDeviceNode);
        closeFramebuffer();
        return false;
    }

    mFramebufferFD = fd;
    return true;
}

bool FrameBuffer::closeFramebuffer() {
    if (mFramebuffer != NULL) {
        int framebufferSize = getWidth() * getHeight() * getBitPerPixel() / 8;
        munmap(mFramebuffer, framebufferSize);
        mFramebuffer = NULL;
    }

    memset(&mFramebufferFixInfo, 0, sizeof(FixInfo));
    memset(&mFramebufferVarInfo, 0, sizeof(VarInfo));
    close(mFramebufferFD);
    mFramebufferFD = -1;
    return true;
}

void FrameBuffer::cleanFramebuffer() {
    if (mFramebuffer == NULL) {
        return;
    }

    int byteSize = getWidth() * getHeight() * getBitPerPixel() / 8;
    memset(mFramebuffer, 0x00, byteSize);
}

void FrameBuffer::fillColor(unsigned int color) {
    int width        = getWidth();
    int height       = getHeight();
    int bitPerPixel  = getBitPerPixel();
    int bytePerPixel = bitPerPixel / 8;
    int bytePerLine  = mFramebufferFixInfo.line_length;
    int byteSize     = getWidth() * getHeight() * getBitPerPixel() / 8;

    switch (bitPerPixel) {
        case 8: {
            int red   = RGBA8888_RED(color);
            int green = RGBA8888_GREEN(color);
            int blue  = RGBA8888_BLUE(color);
            int gray  = (red * 76 + green * 150 + blue * 30) >> 8;
            memset(mFramebuffer, gray, byteSize);
        } break;

        case 16: {
            int red   = RGBA8888_RED(color);
            int green = RGBA8888_GREEN(color);
            int blue  = RGBA8888_BLUE(color);
            int alpha = RGBA8888_ALPHA(color);
            printf("[FillColor] fill RGBA8888(0x%02X%02X%02X%02X) -> RGB565(0x%04X)\n",
                     red, green, blue, alpha, RGB565(red, green, blue));
            for (int h = 0; h < height; h++) {
                for (int w = 0; w < width; w++) {
                    int index = h * bytePerLine + w * bytePerPixel;
                    u16* pixel = (u16*)(mFramebuffer + index);
                    *(pixel) = RGB565(red, green, blue);
                }
            }
        } break;

        case 32: {
            for (int h = 0; h < height; h++) {
                for (int w = 0; w < width; w++) {
                    int index = h * bytePerLine + w * bytePerPixel;
                    u32* pixel = (u32*)(mFramebuffer + index);
                    *(pixel) = color;
                }
            }
        } break;

        default: {
            printf("[FillColor] Unknown pixel format, %d bits per pixel\n", bitPerPixel);
        } break;
    }
}

void FrameBuffer::drawImage(void* pixels, int imageWidth, int imageHeight, int imageBitPerPixel) {
    int width        = getWidth();
    int height       = getHeight();
    int bitPerPixel  = getBitPerPixel();
    int bytePerPixel = bitPerPixel / 8;
    int bytePerLine  = mFramebufferFixInfo.line_length;
    int byteSize     = getWidth() * getHeight() * getBitPerPixel() / 8;

#if DEBUG
        printf("[DrawImage] size is (%d, %d), bits per pixel is %dbits\n",
            imageWidth, imageHeight, imageBitPerPixel);
#endif

    if (bitPerPixel == imageBitPerPixel) {
#if DEBUG
        printf("[DrawImage] has same bitPerPixel(%dbits), just copy mem to framebuffer\n", imageBitPerPixel);
#endif

        u16* framebuffer = (u16*) mFramebuffer;
        u16* pixelbuffer = (u16*) pixels;
        int imageLineBytes = imageWidth * imageBitPerPixel;
        for (int h = 0; h < imageHeight; h++) {
            for (int w = 0; w < imageWidth; w++) {
                int index = h * imageWidth + w;
                framebuffer[index] = pixelbuffer[index];
            }
            usleep(100 * 1000);
        }
    }

    switch (bitPerPixel) {
        case 8: {
            // TODO
        } break;

        case 16: {

        } break;

        case 32: {

        } break;

        default: {
            printf("[DrawImage] Unknown pixel format, %d bits per pixel\n", bitPerPixel);
        } break;
    }
}

