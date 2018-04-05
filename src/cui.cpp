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

//
    game_window = newwin((int) 100, (int) 100, 0, 0);
//    game_window = newwin((int) game_level.size(), (int) game_level[0].size(), 0, 0);
//
    update_game_frame(game_window, current_game);
    game::GameControls player_selection;

    do {
        switch (getch()) {
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
            default:
                break;
        }

//        update_game_frame(game_window, game_level);
    } while(current_game.make_turn(player_selection) != game::GameStatus::stop);

    wclear(game_window);
    wrefresh(game_window);
    delwin(game_window);
}

void cui::Ui::update_game_frame(WINDOW* game_window, const Game& game) const
{
    for (auto map_iterator = game.map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        const std::shared_ptr<const game::MapCell> cur_item = map_iterator->get_item();
        mvwaddch(game_window, cur_item->actor()->row(), cur_item->actor()->col(), static_cast<uint>(cur_item->actor()->map_icon()));
    }
    wrefresh(game_window);


//    for (int i = 0; i < (int) game_level.size(); i++) {
//        for (int j = 0; j < (int) game_level[i].size(); j++) {
//            mvwaddch(game_window,i,j, (uint)game_level[i][j]->floorActor().map_icon());
//            mvwaddch(game_window,i,j, (uint)game_level[i][j]->actor().map_icon());
//        }
//    }
//    wrefresh(game_window);
}
