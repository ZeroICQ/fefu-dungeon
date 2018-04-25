#pragma once

#include <memory>
#include "map.h"

namespace game {

class Game
{
public:
    explicit Game();

    void handle_controls(GameControls control);

    std::unique_ptr<MapConstIterator> map_const_iterator() const { return map_->const_iterator(); }
    std::unique_ptr<MapIterator> map_iterator() const { return map_->iterator(); }

    int get_map_width() const { return map_->width(); }
    int get_map_height() const { return map_->height(); }

private:
    std::shared_ptr<Map> map_;
};

}//namespace game
