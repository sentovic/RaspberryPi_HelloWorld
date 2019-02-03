#include "basic.h"
#include <stdio.h>
#include <X11/keysym.h>

XBase::XBase() : mDisplayInfo(nullptr) {
    XInitThreads();
    if (createDisplay()) {
        detectWindowArguments(mDisplay);
        createFullScreenWindowAndShow(mDisplay, mDisplayInfo);
    }
}

XBase::~XBase() {
    disposeGraphicContext();
    disposeDisplay();
}

bool XBase::createDisplay() {
    mDisplay = XOpenDisplay(NULL);
    if (mDisplay == nullptr) {
        printf("[XBase][createDisplay] XOpenDisplay(DEFAULT) failed!\n");
        return false;
    }

    return true;
}

void XBase::disposeDisplay() {
    if (mDisplayInfo != nullptr) {
        delete mDisplayInfo;
        mDisplayInfo = nullptr;
    }
}

bool XBase::createGraphicContext() {
    if (mXRenderer != nullptr) {
        return false;
    }

    mXRenderer = new XRenderer(mDisplay, mFullScreenWindow);
    mXRenderer->resizeWindow(mDisplayInfo->screenWidth, mDisplayInfo->screenHeight);
    return true;
}

void XBase::disposeGraphicContext() {
    if (mXRenderer != nullptr) {
        delete mXRenderer;
        mXRenderer = nullptr;
    }
}

bool XBase::detectWindowArguments(Display* display) {
    if (display == nullptr) 
        return false;

    const int screenCount = ScreenCount(display);
    const int screenId = DefaultScreen(display);
    const int screenWidth = DisplayWidth(display, screenId);
    const int screenHeight = DisplayHeight(display, screenId);
    const int screenPlanes = DisplayPlanes(display, screenId);
    const unsigned long colorBlack = BlackPixel(display, screenId);
    const unsigned long colorWhite = WhitePixel(display, screenId);

    if (mDisplayInfo == nullptr) {
        mDisplayInfo = new DisplayInfo;
    }
    mDisplayInfo->screenId = screenId;
    mDisplayInfo->screenWidth = screenWidth;
    mDisplayInfo->screenHeight = screenHeight;
    mDisplayInfo->screenPlanes = screenPlanes;
    mDisplayInfo->colorRange[BLACK] = colorBlack;
    mDisplayInfo->colorRange[WHITE] = colorWhite;

    printf("[XBase][detectWindowArguments]\n"
           "  Screen count: %d\n"
           "  Default screen id is #%d\n"
           "  Screen#%d size is %d x %d (x %d)\n"
           "  Screen#%d color range is from %08X(BLACK) to %08X(WHITE)\n",
           screenCount,
           screenId,
           screenId, screenWidth, screenHeight, screenPlanes,
           screenId, colorBlack, colorWhite);

    return true;
}

bool XBase::createFullScreenWindowAndShow(Display* display, DisplayInfo* displayInfo) {
    if ((display == nullptr) || (displayInfo == nullptr))
        return false;

    // Create window
    mFullScreenWindow = XCreateSimpleWindow(
        display, XRootWindow(display, displayInfo->screenId),
        0, 0,                           // x, y
        displayInfo->screenWidth, displayInfo->screenHeight,
        0,                              // border width
        displayInfo->colorRange[BLACK], // border color
        0x00252525                      // background color
        );

    // Setup window attributes
    XSetWindowAttributes attributes = { 0 };
    attributes.override_redirect = True;
    XChangeWindowAttributes(
        display, mFullScreenWindow, 
        CWOverrideRedirect, &attributes);
    
    // Setup events
    XSelectInput(
        display, mFullScreenWindow, 
        StructureNotifyMask 
        | ExposureMask
        | FocusChangeMask
        | KeyPressMask | KeyReleaseMask);

    // Apply to window
    XMapWindow(display, mFullScreenWindow);

    return true;
}

void XBase::loop() {
    if (mDisplay == nullptr) 
        return;

    XEvent event = { 0 };
    while (True) {
        XNextEvent(mDisplay, &event);
        switch (event.type) {
            case CreateNotify: {
                printf("[XBase][loop] Window createed\n");
            } break;

            case DestroyNotify: {
                printf("[XBase][loop] Window destroyed\n");
                XCloseDisplay(mDisplay);
                mDisplay = nullptr;
                // Quit application
                return;
            }

            case MapNotify: {
                printf("[XBase][loop] Window mapped\n");
                createGraphicContext();
            } break;

            case UnmapNotify: {
                printf("[XBase][loop] Window mapped\n");
                disposeGraphicContext();
            } break;
        
            case Expose: {
                printf("[XBase][loop] Window exposed, should redraw\n");
                focusSelf();
                if (mXRenderer != nullptr) {
                    mXRenderer->render();
                }
            } break;

            case GraphicsExpose: {
                printf("[XBase][loop] Graphic exposed, should redraw\n");
                if (mXRenderer != nullptr) {
                    mXRenderer->render();
                }
            } break;

            case FocusIn: {
                printf("[XBase][loop] Window focus changed, get focus\n");
            } break;

            case FocusOut: {
                printf("[XBase][loop] Window focus changed, lose focus\n");

                // If window is overrided redirect, get input focus for it,
                // or it will lose keyboard input focus.
                if ((mDisplay != nullptr) && (mFullScreenWindow != None)) {
                    XWindowAttributes attributes = { 0 };
                    XGetWindowAttributes(mDisplay, mFullScreenWindow, &attributes);
                    if (attributes.override_redirect == True) {
                        focusSelf();
                    }
                }
            } break;

            case KeyPress:
            case KeyRelease: {
                handleKeyEvent(&event.xkey);
            } break;

            default:
                break;
        }
    }
}

bool XBase::focusSelf() {
    if (mFullScreenWindow != None) {
        XSetInputFocus(
            mDisplay, mFullScreenWindow, 
            RevertToPointerRoot, CurrentTime);
        XRaiseWindow(
            mDisplay, mFullScreenWindow);
        return true;
    }
    
    return false;
}

bool XBase::handleKeyEvent(XKeyEvent* event) {
    printf("[XBase][handleKeyEvent] keycode: %d, type: %d\n",
        event->keycode, event->type);
    const int VK_ESCAPE = XKeysymToKeycode(mDisplay, XK_Escape);
    if ((event->type == KeyRelease) && (event->keycode == VK_ESCAPE)) {
        XDestroyWindow(mDisplay, mFullScreenWindow);
        mFullScreenWindow = None;
        return true;
    }

    return false;
}