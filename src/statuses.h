#pragma once

namespace game {
    enum class GameStatus {in_progress, lost, won, level_passed, paused};
    enum class GameControls {move_up, move_down, move_left, move_right, shoot, next_weapon, idle};

}//namespace game
