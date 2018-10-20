#include "../FrameBuffer/framebuffer.h"
#include "png.h"

#define PNG_FILE "/home/pi/Subversion/HelloWorld/PNG/ui.png"

int main() {
    PNG* image = PNG::decodeFile(PNG_FILE, CONFIG_RGB565);
    FrameBuffer* buffer = new FrameBuffer("/dev/fb1");
    buffer->cleanFramebuffer();
    buffer->drawImage(
        image->getPixels(),
        image->getWidth(),
        image->getHeight(),
        image->getBitPerPixel());
    delete buffer;
    delete image;
    return 0;
}
