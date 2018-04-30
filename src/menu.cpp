#include <ncurses.h>
#include "menu.h"


cui::Menu::Menu(const std::string &title, const std::vector<std::string> &entries, int height, int width)
        : title_(title), entries_(entries), height_(height), width_(width),
          selected_(entries.size() > 1 ? 0 : -1)
{
    menu_window_ = newwin(height_, width_, center_y()-height_/2, center_x()-width_/2);
}

cui::Menu::~Menu()
{
    delwin(menu_window_);
}

int cui::Menu::show()
{
    box(menu_window_, 0, 0);

    print_title(menu_window_);

    bool exit = false;

    //must be redrawed for the first time
    int prev_max_x = -1;
    int prev_max_y = -1;

    while (!exit) {
        if (prev_max_x != getmaxx(stdscr) || prev_max_y != getmaxy(stdscr)) {
            prev_max_x = getmaxx(stdscr);
            prev_max_y = getmaxy(stdscr);

            wclear(menu_window_);
            wresize(menu_window_, height_, width_);
            mvwin(menu_window_, center_y() - height_ / 2, center_x() - width_ / 2);
            wclear(stdscr);
        }
        //TODO: why?
        box(menu_window_, 0, 0);
        print_title(menu_window_);

        for (int i = 0; i < (int) entries_.size(); i++) {
            if (selected_ == i) {
                wattron(menu_window_, WA_REVERSE);
            }

            mvwprintw(menu_window_, 2 + i, static_cast<int>(width_ - entries_[i].length())/2, entries_[i].c_str());
            wattroff(menu_window_, A_REVERSE);
        }

        wrefresh(menu_window_);


        switch (getch()) {
            case ERR:
                continue;
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
