#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include "actors.h"
#include "map.h"

namespace game {

enum class GameStatus {in_progress, stop};
enum class GameControls {move_up, move_down, move_left, move_right};


class Game
{
public:
    Game();
    GameStatus make_turn(GameControls control);
private:
     std::unique_ptr<Map> map_;
};

}//namespace game
