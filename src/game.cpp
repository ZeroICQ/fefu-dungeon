#include "game.h"

using std::vector;


game::Game::Game()
{
    const std::vector<std::string> map = {
            "###################################################################",
            "#                                                                 #",
            "#                                                               G #",
            "#                #    #   #######   #    #                        #",
            "#                #   #    #         #   #                         #",
            "#                #  #     #  G      #  #                          #",
            "#                # #      #         # #                           #",
            "#         G      ##GS     #######   ##                            #",
            "#                # #      #         # #                           #",
            "#                #  #     #         #  #                          #",
            "#                #   #    #         #   #                         #",
            "#                #    #   #         #    #                        #",
            "#          G     #     #  #######   #     #                       #",
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
        map_iterator->actor()->move(control, map_);
    }

    EventManager::instance().trigger_all(map_);

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        auto curr_actor = map_iterator->actor();

        if (curr_actor->is_dead()) {
            if (curr_actor->is_playable()) {
                status_ = GameStatus::lost;
            }
            map_->remove_actor(curr_actor->row(), curr_actor->col());
        }
    }

}

std::shared_ptr<game::Actor> game::Game::get_main_char()
{
    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        if (map_iterator->actor()->is_playable()) {
            return map_iterator->actor();
        }
    }

    //meh...
    return nullptr;
}
