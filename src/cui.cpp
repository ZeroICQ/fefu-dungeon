#include <string>
#include <curses.h>
#include <cstring>
#include <vector>
#include "cui.h"
#include "menu.h"
#include "game.h"

using game::Game;

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
                while (true) {

                    start_game();
                }
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

void cui::Ui::start_game() const
{
    while (1) {
        Game new_game;
    }
//    new_game.start();
//
//    while(new_game.send_control(getch()) != 0) {
//        new_game.start();
//    }
}
