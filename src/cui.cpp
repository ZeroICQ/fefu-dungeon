#include <string>
#include <curses.h>
#include <cstring>
#include <vector>
#include "cui.h"
#include "menu.h"
#include "game.h"

using game::Game;

void cui::Ui::show_menu()
{
    initialize_curses();
    MainMenu main_menu;

    int choice = -1;

    while ((choice = main_menu.show()) > -1) {
        switch (choice) {
            case MainMenu::Actions::start:
                main_menu.clear();
                start_game();
                break;
            case MainMenu::Actions::exit:
                return;
            default:
                break;
        }
    }

}

cui::Ui::~Ui()
{
    endwin();
}

void cui::Ui::initialize_curses() const
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
    WINDOW *game_window;

    Game new_game;

    auto game_level = new_game.get_level();
    game_window = newwin((int) game_level.size(), (int) game_level[0].size(), 0, 0);

    update_game_frame(game_window, game_level);
    game::GameControls player_selection;

    do {
        switch (getch()) {
            case 's':
            case KEY_DOWN:
                player_selection = game::GameControls::down;
                break;
            case 'a':
            case KEY_LEFT:
                player_selection = game::GameControls::left;
                break;
            case 'w':
            case KEY_UP:
                player_selection = game::GameControls::up;
            case 'd':
            case KEY_RIGHT:
                player_selection = game::GameControls::right;
            default:
                break;
        }

        update_game_frame(game_window, game_level);
    } while(new_game.make_turn(player_selection) != game::GameStatus::stop);

    wclear(game_window);
    wrefresh(game_window);
    delwin(game_window);
}

void cui::Ui::update_game_frame(WINDOW* game_window, std::vector<std::vector<game::MapCell*>>& game_level) const
{
    for (int i = 0; i < (int) game_level.size(); i++) {
        for (int j = 0; j < (int) game_level[i].size(); j++) {
            mvwaddch(game_window,i,j, (uint)game_level[i][j]->floorActor().map_icon());
            mvwaddch(game_window,i,j, (uint)game_level[i][j]->actor().map_icon());
        }
    }
    wrefresh(game_window);
}
