#include <ncurses.h>
#include "colors.h"

void game::Colors::initialize()
{
    init_pair(FULL_WHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(WHITE_CYAN, COLOR_WHITE, COLOR_CYAN);
    init_pair(WHITE_RED, COLOR_WHITE, COLOR_RED);
    init_pair(WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
}
