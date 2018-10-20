#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdbool.h>  // bool
#include <vector>   // vector
#include <linux/fb.h> // fb_fix_screeninfo, fb_var_screeninfo

typedef struct fb_fix_screeninfo FixInfo;
typedef struct fb_var_screeninfo VarInfo;
typedef char Byte;

class FrameBuffer {
public:
    static int enumerateFrameBufferDevice(const char** buffer);

public:
    FrameBuffer(const char* deviceNode);
    ~FrameBuffer();

    inline int getWidth() { return mFramebufferVarInfo.xres; }
    inline int getHeight() { return mFramebufferVarInfo.yres; }
    inline int getBitPerPixel() { return mFramebufferVarInfo.bits_per_pixel; }

    void cleanFramebuffer();
    void fillColor(unsigned int color);
    void drawImage(void* pixels, int width, int height, int bitPerPixel);

protected:
    bool openFramebuffer();
    bool closeFramebuffer();

private:
    char* mDeviceNode;
    int mFramebufferFD;
    Byte* mFramebuffer;
    FixInfo mFramebufferFixInfo;
    VarInfo mFramebufferVarInfo;
};

#endif
