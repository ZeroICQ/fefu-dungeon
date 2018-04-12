#include "game.h"

using std::vector;


game::Game::Game()
{
    const std::vector<std::string> map = {
            "###################################################################",
            "#                                                                 #",
            "#                                                                 #",
            "#                #    #   #######   #    #                        #",
            "#                #   #    #         #   #                         #",
            "#                #  #     #  G      #  #                          #",
            "#                # #      #         # #                           #",
            "#         G      ##       #######   ##        S                   #",
            "#                # #      #         # #                           #",
            "#                #  #     #         #  #                          #",
            "#                #   #    #         #   #                         #",
            "#                #    #   #         #    #                        #",
            "#                #     #  #######   #     #                       #",
            "#                                                                 #",
            "#                                                                 #",
            "#                           G                                     #",
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
    //reset
    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        map_iterator->actor()->can_make_turn(true);
    }

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {

        map_iterator->actor()->move(control, *map_);
    }

    return GameStatus::in_progress;
}
