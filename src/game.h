#pragma once

#include <memory>
#include "map.h"
#include "statuses.h"

namespace game {

class Game
{
public:
    explicit Game();
    GameStatus make_turn(GameControls control);
    std::unique_ptr<MapConstIterator> map_const_iterator() const { return map_->const_iterator(); }
    std::unique_ptr<MapIterator> map_iterator() const { return map_->iterator(); }
    int get_map_width() const { return map_->width(); }
    int get_map_height() const { return map_->height(); }

private:
     std::unique_ptr<Map> map_;
};

}//namespace game
