#include <iostream>
#include "easymusic.h"
#include "system.h"
#include "log.h"

#include <X11/Xlib.h>

void showWindow() {
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        return;
    }

    int screenCount = DefaultScreen(display);
    uint32_t white = WhitePixel(display, screenCount);
    uint32_t black = BlackPixel(display, screenCount);
    Window window = XCreateSimpleWindow(
        display, 
        DefaultRootWindow(display),
        50, 50, 
        200, 200,
        0, white,
        0xFFFFFF00);
    XMapWindow(display, window);
    
    
    XEvent event;
    do { XNextEvent(display, &event); } while(event.type != MapNotify);

    GC gc = XCreateGC(display, window, 0, NULL );
    XSetForeground( display, gc, black );
    XDrawLine(display, window, gc, 10, 10,190,190);
    XFlushGC(display, gc);
    XFlush(display);
}

int main() {
    cobox::Log::i("EasyMusic", "[EasyMusic] launch time is %ld", cobox::System::millsecond());
    showWindow();

    EasyMusic easyMusic;
    easyMusic.loop();

    cobox::Log::i("EasyMusic", "[EasyMusic] quit");
    return 0;
}