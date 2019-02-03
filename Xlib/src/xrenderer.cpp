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
        while (true) {
            std::chrono::milliseconds duration(500);
            std::this_thread::sleep_for(duration);
        
            XEvent event = { 0 };
            event.type = GraphicsExpose;
            event.xgraphicsexpose.send_event = True;
            event.xgraphicsexpose.display = this->mDisplay;
            event.xgraphicsexpose.drawable = this->mWindow;
            event.xgraphicsexpose.width = this->mWindowWidth;
            event.xgraphicsexpose.height = this->mWindowHeight;
            event.xgraphicsexpose.x = 0;
            event.xgraphicsexpose.y = 0;

            Status status = XSendEvent(this->mDisplay, this->mWindow, 
                False, ExposureMask, &event);
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
    
    auto tt = std::chrono::system_clock::to_time_t
	(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&tt);
	char text[64] = { 0 };
	sprintf(text, "%04d-%02d-%02d %02d:%02d:%02d %d",
		(int) ptm->tm_year + 1900,
        (int) ptm->tm_mon + 1,
        (int) ptm->tm_mday,
        (int) ptm->tm_hour,
        (int) ptm->tm_min,
        (int) ptm->tm_sec,
        i++);

    printf("[XRenderer] i = %d\n", i);
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