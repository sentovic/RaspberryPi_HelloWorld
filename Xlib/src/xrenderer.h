#pragma once
#include <X11/Xlib.h>

class XRenderer {
public:
    XRenderer(Display* display, Window window);
    ~XRenderer();

    void resizeWindow(int width, int height);
    bool render();

protected:
    bool isGCReady();
    bool onRenderer();

private:
    bool installFonts();
    void initializeRenderThread();

private:
    Display* mDisplay;
    Window mWindow;
    GC mGc;
    int mWindowWidth;
    int mWindowHeight;
};