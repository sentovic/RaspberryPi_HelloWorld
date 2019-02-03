#include <stdio.h>
#include "basic.h"

int main() {
    printf("XSystem Demo\n");
    XBase* base = new XBase;
    base->loop();
    delete base;
    return 0;
}