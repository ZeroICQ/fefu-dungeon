#pragma once

#include <memory>
#include <chrono>
#include "map.h"
#include "statuses.h"
#include "map_loader.h"

namespace game {

class Game
{
public:
    explicit Game();

    void handle_controls(GameControls control, const std::chrono::time_point<std::chrono::steady_clock>& curr_tp);
    std::unique_ptr<MapConstIterator> map_const_iterator() const { return map_->const_iterator(); }
    std::unique_ptr<MapIterator> map_iterator() const { return map_->iterator(); }

    int get_map_width() const { return map_->width(); }
    int get_map_height() const { return map_->height(); }

    GameStatus status() { return status_; }
    void status(GameStatus n_status);

    std::shared_ptr<Actor> get_main_char() const;

    void reach_target() { is_target_reached_ = true; }

    void load_next_level();

private:
    std::shared_ptr<Map> map_;
    GameStatus status_ = GameStatus::in_progress;

    int level_number_;
    game::MapLoader map_loader_;
    bool is_target_reached_ = false;
};

}//namespace game
