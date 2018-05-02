#include "colors.h"
#include <ncurses.h>

void game::Colors::initialize()
{
    init_pair(FULL_WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
}
