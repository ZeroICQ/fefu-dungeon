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
    nodelay(stdscr, true);// no delay moe
    refresh();
}

void cui::Ui::start_game() const
{
    WINDOW* game_window;
    WINDOW* status_window;

    Game current_game;

    status_window = newwin(0, 0, 0, 0);
    box(status_window, 0, 0);

    game_window = newwin(0, 0, 0, 0);

    update_game_frame(game_window, status_window, current_game, true);
    game::GameControls player_selection = game::GameControls::idle;

    int prev_max_x = getmaxx(stdscr);
    int prev_max_y = getmaxy(stdscr);
    bool is_resized;

    bool exit_game = false;
    do {
        is_resized = false;

        switch (getch()) {
            case 'q':
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

        if (prev_max_x != getmaxx(stdscr) || prev_max_y != getmaxy(stdscr)) {
            prev_max_x = getmaxx(stdscr);
            prev_max_y = getmaxy(stdscr);

            is_resized = true;
        }

        current_game.handle_controls(player_selection);
        update_game_frame(game_window, status_window, current_game, is_resized);


    } while(!exit_game);

    wclear(game_window);
    wrefresh(game_window);

    wclear(status_window);
    wrefresh(status_window);

    delwin(game_window);
    delwin(status_window);
}

void cui::Ui::update_game_frame(WINDOW* game_window, WINDOW* status_window, const Game& game, bool is_resized) const
{
//    newwin(map_height, map_width, (LINES - map_height) / 2, (COLS - map_width) / 2);

    if (is_resized) {
        wresize(status_window, LINES, STATUS_MENU_WIDTH);
        mvwin(status_window, 0, COLS - STATUS_MENU_WIDTH);
        wclear(status_window);

        wresize(game_window,
                std::min(game.get_map_height(), LINES),
                std::min(game.get_map_width(), COLS - STATUS_MENU_WIDTH));

        //center game window
        mvwin(game_window, std::max((LINES - game.get_map_height()) / 2, 0), getbegx(game_window));
        mvwin(game_window, getbegy(game_window), std::max((COLS - game.get_map_width() - STATUS_MENU_WIDTH) / 2, 0));

        wclear(stdscr);
    }

    box(status_window, 0 , 0);

    mvwaddstr(status_window, 0,0, "012345678901234567890123456789");

    for (auto map_iterator = game.map_const_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        //ASK: definition in loop
        mvwaddch(game_window, map_iterator->floor()->row(), map_iterator->floor()->col(),
                 static_cast<uint>(map_iterator->floor()->map_icon()));

        mvwaddch(game_window, map_iterator->actor()->row(), map_iterator->actor()->col(),
                 static_cast<uint>(map_iterator->actor()->map_icon()));
    }

    overwrite(game_window, status_window);
    wrefresh(game_window);
    wrefresh(status_window);
}
