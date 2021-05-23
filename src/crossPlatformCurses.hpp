#if defined(_WIN32) || defined(WIN32)
    #include "curses.h"
#else
    #include <ncurses.h>
#endif