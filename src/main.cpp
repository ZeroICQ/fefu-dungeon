#include <exception>
#include "cui.h"
#include <ncurses.h>

int main()
{
    cui::Ui ui;
    ui.show_menu();

    return 0;
}
