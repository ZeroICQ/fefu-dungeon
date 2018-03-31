#include <exception>
#include "cui.h"
#include <ncurses.h>

int main()
{
    try {
        cui::Ui ui;
        ui.show_menu();
    } catch (const std::exception& e) {
        endwin();
    }

    return 0;
}
