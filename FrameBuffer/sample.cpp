#include "framebuffer.h"
#include <unistd.h>
#include <stdio.h>

#define TIMEOUT 1

int main() {
    FrameBuffer* buffer = new FrameBuffer("/dev/fb1");
    printf("[/dev/fb1] size is (%d, %d), a pixel has %dbits\n",
        buffer->getWidth(), buffer->getHeight(),
        buffer->getBitPerPixel());
    printf("[/dev/fb1] clean screen\n");
    buffer->cleanFramebuffer();

    printf("[/dev/fb1] fill 0xFF0000FF\n");
    buffer->fillColor(0xFF0000FFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0x00FF00FF\n");
    buffer->fillColor(0x00FF00FFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0x0000FFFF\n");
    buffer->fillColor(0x0000FFFFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0xFF00FFFF\n");
    buffer->fillColor(0xFF00FFFFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0xFFFF00FF\n");
    buffer->fillColor(0xFFFF00FFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0x00FFFFFF\n");
    buffer->fillColor(0x00FFFFFFUL); sleep(TIMEOUT);

    printf("[/dev/fb1] fill 0x000000FF\n");
    buffer->fillColor(0x000000FFUL); sleep(TIMEOUT);

    delete buffer;
    return 0;
}
