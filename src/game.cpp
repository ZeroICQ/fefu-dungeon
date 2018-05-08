#include "game.h"
#include "event_system.h"
#include "map_loader.h"

using std::vector;


game::Game::Game() : level_number_(0)
{
    map_ = map_loader_.load_map(0);
}

void game::Game::load_next_level()
{
    map_ = map_loader_.load_map(++level_number_);
    status(GameStatus::in_progress);
    is_target_reached_ = false;
}

void game::Game::handle_controls(game::GameControls control)
{
    if (control == game::GameControls::idle) {
        return;
    }

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        map_iterator->actor()->move(control, map_);
    }

    EventManager::instance().trigger_all(*this, map_);

    if (is_target_reached_) {
        status(GameStatus::won);
    }

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        auto curr_actor = map_iterator->actor();

        if (curr_actor->is_dead()) {

            if (curr_actor->is_playable()) {
                status(GameStatus::lost);
            }
            map_->remove_actor(curr_actor->row(), curr_actor->col());
        }
    }

    if (status() == GameStatus::won && level_number_ + 1  < map_loader_.size()) {
        status(GameStatus::level_passed);
    }
}

std::shared_ptr<game::Actor> game::Game::get_main_char() const
{
    return map_->get_main_char();
}

void game::Game::status(game::GameStatus n_status)
{
        status_ = n_status;
}
