#ifndef _API
#define _API

#include "vga.h"

enum modes {
    MODE_TEXT,
    MODE_GRAPHIC,
};

void parse_input(char ch);

#endif // _API