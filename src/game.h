#pragma once

#include <memory>
#include "map.h"
#include "statuses.h"

namespace game {
class MapConstIterator;

class Game
{
public:
    explicit Game();

    void handle_controls(GameControls control);

    std::unique_ptr<MapConstIterator> map_const_iterator() const { return map_->const_iterator(); }
    std::unique_ptr<MapIterator> map_iterator() const { return map_->iterator(); }

    int get_map_width() const { return map_->width(); }
    int get_map_height() const { return map_->height(); }

    GameStatus status() { return status_; }
    void status(GameStatus n_status);

    std::shared_ptr<Actor> get_main_char();

private:
    std::shared_ptr<Map> map_;
    GameStatus status_ = GameStatus::in_progress;
};

}//namespace game
