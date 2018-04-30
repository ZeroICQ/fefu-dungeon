#include "colors.h"
#include <ncurses.h>

void game::Colors::initialize()
{
    init_pair(FULL_WHITE, COLOR_WHITE, COLOR_WHITE);
}
