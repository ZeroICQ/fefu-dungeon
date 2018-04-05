#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include "map.h"

namespace game {

enum class GameStatus {in_progress, stop};
enum class GameControls {move_up, move_down, move_left, move_right};


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
