#pragma once
#include <X11/Xlib.h>
#include "xrenderer.h"

enum ColorRange {
    BLACK = 0,
    WHITE = 1
};

struct DisplayInfo {
    int screenId;
    int screenWidth;
    int screenHeight;
    int screenPlanes;
    unsigned long colorRange[2];
};

class XBase {
public:
    XBase();
    ~XBase();

    void loop();

private:
    bool createDisplay();
    void disposeDisplay();
    bool createGraphicContext();
    void disposeGraphicContext();
    bool detectWindowArguments(Display* display);
    bool createFullScreenWindowAndShow(Display* display, DisplayInfo* displayInfo);
    bool handleKeyEvent(XKeyEvent* event);
    bool focusSelf();

private:
    Display* mDisplay;
    DisplayInfo* mDisplayInfo;
    Window mFullScreenWindow;
    XRenderer* mXRenderer;
};