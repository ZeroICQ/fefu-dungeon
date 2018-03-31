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

    main_menu.show();
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