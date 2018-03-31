#include <string>
#include <curses.h>
#include <cstring>
#include <vector>
#include "cui.h"
#include "menu.h"

cui::Ui::~Ui()
{
    endwin();
}

void cui::Ui::show_menu()
{
    initialize();
    MainMenu main_menu;

    int choice = -1;

    while ((choice = main_menu.show()) > -1) {
        switch (choice) {
            case MainMenu::Actions::start:
                break;
            case MainMenu::Actions::exit:
                return;
            default:
                break;
        }
    }

}

void cui::Ui::initialize() const
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);//set cursor invisible
    refresh();
}
