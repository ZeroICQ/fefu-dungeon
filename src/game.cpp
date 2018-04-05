#include "game.h"
#include "actors.h"

using std::vector;


game::Game::Game()
{
    const std::vector<std::string> map = {
            "###################################################################",
            "#                                                                 #",
            "#           ###########                                           #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                             #####################               #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "###################################################################"};

    const std::vector<std::string> floor_map = {
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   "};

    map_.reset(new Map(map, floor_map));
}


game::GameStatus game::Game::make_turn(game::GameControls control)
{
    return GameStatus::in_progress;
}
