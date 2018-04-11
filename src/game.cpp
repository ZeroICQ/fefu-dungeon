#include "game.h"

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
            "#                                  S                              #",
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
    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        //TODO: why modify if const
        map_iterator->actor()->move(control);
    }

    return GameStatus::in_progress;
}
