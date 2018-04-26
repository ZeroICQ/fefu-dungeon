#include <caca.h>
#include "cui.h"
#include "menu.h"
#include "game.h"
#include "map.h"

using game::Game;
using std::unique_ptr;

cui::Ui::~Ui()
{
    endwin();
}

void cui::Ui::run()
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
    Game current_game;

    int map_height = current_game.get_map_height();
    int map_width = current_game.get_map_width();

    game_window = newwin(map_height, map_width, (LINES - map_height) / 2, (COLS - map_width) / 2);
    update_game_frame(game_window, current_game);
    game::GameControls player_selection = game::GameControls::idle;

    bool exit_game = false;
    do {
        switch (getch()) {
            case 'q':
            case KEY_A1:
                exit_game = true;
                break;
            case 's':
            case KEY_DOWN:
                player_selection = game::GameControls::move_down;
                break;
            case 'a':
            case KEY_LEFT:
                player_selection = game::GameControls::move_left;
                break;
            case 'w':
            case KEY_UP:
                player_selection = game::GameControls::move_up;
                break;
            case 'd':
            case KEY_RIGHT:
                player_selection = game::GameControls::move_right;
                break;
            default:
                player_selection = game::GameControls::idle;
                break;
        }

        current_game.handle_controls(player_selection);
        update_game_frame(game_window, current_game);
    } while(!exit_game);

    wclear(game_window);
    wrefresh(game_window);
    delwin(game_window);
}

void cui::Ui::update_game_frame(WINDOW* game_window, const Game& game) const
{

    for (auto map_iterator = game.map_const_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        //ASK: definition in loop
        mvwaddch(game_window, map_iterator->floor()->row(), map_iterator->floor()->col(),
                 static_cast<uint>(map_iterator->floor()->map_icon()));

        mvwaddch(game_window, map_iterator->actor()->row(), map_iterator->actor()->col(),
                 static_cast<uint>(map_iterator->actor()->map_icon()));
    }
    wrefresh(game_window);
}
