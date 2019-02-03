#include "xrenderer.h"
#include <string.h>
#include <stdio.h>
#include <thread>
#include <chrono>

XRenderer::XRenderer(Display* display, Window window) :
                                                    mDisplay(display),
                                                    mWindow(window),
                                                    mWindowWidth(0),
                                                    mWindowHeight(0) {
    XGCValues values = { 0 };
    values.cap_style = CapButt;
    values.join_style = JoinBevel;
    values.foreground = 0x0000FF00;
    values.background = 0x00252525;

    mGc = XCreateGC(
        display, window,
        GCCapStyle | GCJoinStyle,
        &values);

    installFonts();
    initializeRenderThread();
}

XRenderer::~XRenderer() {
    mWindowWidth = 0;
    mWindowHeight = 0;
    if ((mDisplay != nullptr) && (mGc != nullptr)) {
        XFreeGC(mDisplay, mGc);
        mDisplay = nullptr;
        mGc = nullptr;
    }
}

bool XRenderer::installFonts() {
    if (isGCReady()) {
        // TODO
    }
}

void XRenderer::initializeRenderThread() {
    std::thread {[this](){
        printf("[RenderThread]\n");
        while (true) {
            std::chrono::milliseconds duration(500);
            std::this_thread::sleep_for(duration);
            XSendEvent(this->mDisplay, this->mWindow)
        }
    }}.detach();
}

bool XRenderer::isGCReady() {
    return (mDisplay != nullptr) && (mGc != nullptr);
}

void XRenderer::resizeWindow(int width, int height) {
    mWindowWidth = width;
    mWindowHeight = height;
}

bool XRenderer::render() {
    if (!isGCReady()) {
        return false;
    }

    XClearWindow(mDisplay, mWindow);
    bool hasMoreFrame = onRenderer();
    XFlush(mDisplay);
    return hasMoreFrame;
}

bool XRenderer::onRenderer() {
    XFontStruct* font = XLoadQueryFont(mDisplay, "-adobe-courier-bold-r-normal--*-480-*-*-m-*-*-*");
    if (font != nullptr) {
        XSetFont(mDisplay, mGc, font->fid);
    } else {
        XSetFont(mDisplay, mGc, XLoadQueryFont(mDisplay, "fixed")->fid);
    }

    int x, y;
    int direction;
    int ascent, descent;
    XCharStruct overall;
    char text[256] = { 0 };
    
    uint64_t tictoc = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
    long hour = (tictoc / 1000 / 60 / 60) % 24;
    long minute = (tictoc / 1000 / 60) % 60;
    long second = (tictoc / 1000) % 60;
    sprintf(text, "%02d:%02d:%02d", 
        hour, minute, second
    );

    int textLength = strlen(text);

    XSetForeground(mDisplay, mGc, 0x0000FF00);
    XSetBackground(mDisplay, mGc, 0x00000000);
    XTextExtents(
        font, text, textLength,
        &direction, &ascent, &descent, &overall);
    x = (mWindowWidth - overall.width) / 2;
    y = mWindowHeight / 2 + (ascent - descent) / 2;
    XClearWindow(mDisplay, mWindow);
    XDrawString(
        mDisplay, mWindow, mGc,
        x, y, text, textLength);
}