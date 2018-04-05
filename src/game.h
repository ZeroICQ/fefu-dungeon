#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include "map.h"
#include "statuses.h"

namespace game {

class Game
{
public:
    explicit Game();
    GameStatus make_turn(GameControls control);
    std::unique_ptr<MapConstIterator> map_iterator() const { return map_->iterator(); }
private:
     std::unique_ptr<Map> map_;
};

}//namespace game
