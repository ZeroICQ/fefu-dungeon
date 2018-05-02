#include <cmath>
#include "cui.h"
#include "menu.h"
#include "game.h"
#include "map.h"
#include "colors.h"
#include "statuses.h"

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
    start_color();
    refresh();

    //define color_pairs
    game::Colors::initialize();
}

void cui::Ui::start_game() const
{
    WINDOW* game_window;
    WINDOW* status_window;

    Game current_game;

    status_window = newwin(0, 0, 0, 0);
    game_window = newwin(0, 0, 0, 0);

    auto main_char = current_game.get_main_char();

    update_game_frame(game_window, status_window, current_game, main_char, true);
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

        //detect terminal resize
        if (prev_max_x != getmaxx(stdscr) || prev_max_y != getmaxy(stdscr)) {
            prev_max_x = getmaxx(stdscr);
            prev_max_y = getmaxy(stdscr);

            is_resized = true;
        }

        current_game.handle_controls(player_selection);
        update_game_frame(game_window, status_window, current_game, main_char, is_resized);

        exit_game = current_game.status() != game::GameStatus::in_progress ? true : exit_game;
    } while(!exit_game);

    if (current_game.status() == game::GameStatus::lost) {
        print_message(game_window, "LOL, YOU DIED!");
    } else if (current_game.status() == game::GameStatus::won) {
        print_message(game_window, "YOU WON!");
    }

    while (getch() == ERR) {
        //wait for input
    }

    wclear(game_window);
    wrefresh(game_window);

    wclear(status_window);
    wrefresh(status_window);

    delwin(game_window);
    delwin(status_window);
}

void cui::Ui::update_game_frame(WINDOW* game_window, WINDOW* status_window, const Game& game,
                                std::shared_ptr<game::Actor> main_char, bool is_resized) const
{
    update_status_window(status_window, game, main_char, is_resized);
    update_game_window(game_window, game, main_char, is_resized);

    if (is_resized) {
        wclear(stdscr);
    }

    overwrite(game_window, status_window);
    wrefresh(game_window);
    wrefresh(status_window);
}

void cui::Ui::update_game_window(WINDOW* game_window, const game::Game& game,
                                 std::shared_ptr<game::Actor> main_char, bool is_resized) const
{
    if (is_resized) {
        wresize(game_window,
                std::min(game.get_map_height(), LINES),
                std::min(game.get_map_width(), COLS - STATUS_MENU_WIDTH));

        mvwin(game_window, std::max((LINES - game.get_map_height()) / 2, 0), getbegx(game_window));
        mvwin(game_window, getbegy(game_window), std::max((COLS - game.get_map_width() - STATUS_MENU_WIDTH) / 2, 0));
    }

    //center camera on main character
    int start_row = main_char->row() - getmaxy(game_window) / 2;
    int start_col = main_char->col() - getmaxx(game_window) / 2;

    //top-left bounds
    start_row = std::max(start_row, 0);
    start_col = std::max(start_col, 0);
    //bot-right bounds
    start_row = std::min(start_row, game.get_map_height() - getmaxy(game_window));
    start_col = std::min(start_col, game.get_map_width()  - getmaxx(game_window));

    for (auto map_iterator = game.map_const_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        auto curr_actor = map_iterator->actor();
        auto curr_floor = map_iterator->floor();

        //bounds check
        if (curr_actor->row() < start_row || curr_actor->col() < start_col
            || curr_actor->col() >= start_col + getmaxx(game_window)) {
            continue;
        }

        if (curr_actor->row() >= start_row + getmaxy(game_window)) {
            break;
        }

        //print map characters
        wattron(game_window, COLOR_PAIR(curr_floor->color_pair()));
        mvwaddch(game_window, map_iterator->floor()->row() - start_row, map_iterator->floor()->col() - start_col,
                 static_cast<uint>(map_iterator->floor()->map_icon()));
        wattroff(game_window, COLOR_PAIR(curr_floor->color_pair()));

        wattron(game_window, COLOR_PAIR(curr_actor->color_pair()));
        mvwaddch(game_window, map_iterator->actor()->row() - start_row, map_iterator->actor()->col() - start_col,
                 static_cast<uint>(map_iterator->actor()->map_icon()));
        wattroff(game_window, COLOR_PAIR(curr_actor->color_pair()));
    }
}

void

cui::Ui::update_status_window(WINDOW *status_window, const game::Game &game, std::shared_ptr<game::Actor> main_char,
                              bool is_resized) const
{
    if (is_resized) {
        wresize(status_window, LINES, STATUS_MENU_WIDTH);
        mvwin(status_window, 0, COLS - STATUS_MENU_WIDTH);
        wclear(status_window);
    }

    touchwin(status_window);
    box(status_window, 0 , 0);

    //fill status window
    print_param_status(status_window, 1, 1, std::string(1, main_char->map_icon()), "FEFU student");
    print_param_status(status_window, 2, 1, "HP", std::to_string(main_char->curr_hp()));
    print_progressbar(status_window, 3 , 1, main_char->curr_hp(), main_char->max_hp());

}

void cui::Ui::print_param_status(WINDOW* status_window, int row, int col, const std::string& name, const std::string& value) const
{
    auto name_width =  static_cast<int>(name.length());
    auto value_width =  static_cast<int>(value.length());

    int window_width = getmaxx(status_window);
    //-1 for border
    int dots_width = window_width - col - name_width - value_width - 1;
    dots_width = std::max(dots_width, 0);

    mvwaddstr(status_window, row, col, name.c_str());
    mvwaddstr(status_window, row, window_width - value_width - 1, value.c_str());

    mvwhline(status_window, row, col + name_width, '.', dots_width);

}

void cui::Ui::print_progressbar(WINDOW* status_window, int row, int col, int val, int max) const
{
    int window_width = getmaxx(status_window);
    //-1 for border
    int bar_max_width = std::max(window_width - col - 1, 0);

    auto bar_cur_width = static_cast<int>(std::ceil(static_cast<double>(val) / max * bar_max_width));

    mvwhline(status_window, row, col, '-', bar_max_width);
//    wattron(status_window, COLOR_PAIR(game::Colors::FULL_WHITE));
    mvwhline(status_window, row, col, '#', bar_cur_width);
//    wattroff(status_window, COLOR_PAIR(game::Colors::FULL_WHITE));
}

void cui::Ui::print_message(WINDOW* window, std::string message) const
{
    wclear(window);
    int center_row = getmaxy(window) / 2;
    int center_col = (getmaxx(window) - static_cast<int>(message.length())) / 2;
    mvwaddstr(window, center_row, center_col, message.c_str());
    wrefresh(window);
}
