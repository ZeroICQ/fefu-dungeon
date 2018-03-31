#include <ncurses.h>
#include "menu.h"


cui::Menu::~Menu()
{
    delwin(menu_window_);
}

int cui::Menu::show()
{
    box(menu_window_, 0, 0);

    print_title(menu_window_);

    bool exit = false;

    while (!exit) {
        for (int i = 0; i < (int) entries_.size(); i++) {
            if (selected_ == i) {
                wattron(menu_window_, WA_REVERSE);
            }

            mvwprintw(menu_window_, 1 + i, 1, entries_[i].c_str());
            wattroff(menu_window_, A_REVERSE);
        }

        wrefresh(menu_window_);
        int c;
        switch ((c = getch())) {
            case KEY_F(1):
            case 'e':
                return -1;
            case KEY_ENTER:
            case REAL_KEY_ENTER:
                exit = true;
                break;
            case KEY_DOWN:
            case 's':
                select_next();
                break;
            case KEY_UP:
            case 'w':
                select_prev();
            default:
                break;
        }
    }

    return selected_;
}

void cui::Menu::print_title(WINDOW *window) const
{
    mvwprintw(window, 0, (width_ - (int) title_.length()) / 2, title_.c_str());
}

void cui::Menu::select_next()
{
    if (selected_ < (int) entries_.size() - 1) {
        selected_++;
    }
}

void cui::Menu::select_prev()

{
    if (selected_> 0) {
        selected_--;
    }
}
