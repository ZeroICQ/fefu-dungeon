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


void game::Game::handle_controls(game::GameControls control)
{
    if (control == game::GameControls::idle) {
        return;
    }

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
//        map_iterator->actor()->move(control, *map_);
    }
}
