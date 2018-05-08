//#include <unistd.h>
#include <libconfig.h++>
#include "game.h"
#include "event_system.h"
using std::vector;


game::Game::Game()
{
//    const std::vector<std::string> map = {
//            "###################################################################",
//            "#    T                                                            #",
//            "#                                                               G #",
//            "#                #    #   #######   #    #                        #",
//            "#                #   #    #         #   #                         #",
//            "#                #  #     #         #  #                          #",
//            "#                # #      #         # #               Z           #",
//            "#                ##GS     #######   ##                            #",
//            "#                # #      #         # #                           #",
//            "#                #  #     #         #  #                          #",
//            "#                #   #    #         #   #                         #",
//            "#                #    #   #         #    #                        #",
//            "#                #     #  #######   #     #                       #",
//            "#                                                                 #",
//            "#                                                                 #",
//            "#   G                                                   T         #",
//            "#                                                                 #",
//            "###################################################################"};
//
//    const std::vector<std::string> floor_map = {
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   ",
//            "                                                                   "};
//    char cwd[1024];
//    getcwd(cwd, sizeof(cwd));

    libconfig::Config level_cfg;
    level_cfg.readFile("../levels/test_1");
    std::string level_map = level_cfg.lookup("map");
    std::string level_floor_map = level_cfg.lookup("floor_map");

    map_.reset(new Map(level_map, level_floor_map);
}


void game::Game::handle_controls(game::GameControls control)
{
    if (control == game::GameControls::idle) {
        return;
    }

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        map_iterator->actor()->move(control, map_);
    }

    EventManager::instance().trigger_all(*this, map_);

    for (auto map_iterator = this->map_iterator(); !map_iterator->is_end(); map_iterator->next()) {
        auto curr_actor = map_iterator->actor();

        if (curr_actor->is_dead()) {

            if (curr_actor->is_playable()) {
                status(GameStatus::lost);
            }
            map_->remove_actor(curr_actor->row(), curr_actor->col());
        }
    }

}

std::shared_ptr<game::Actor> game::Game::get_main_char()
{
    return map_->get_main_char();
}

void game::Game::status(game::GameStatus n_status)
{
    if (status_ != GameStatus::won) {
        status_ = n_status;
    }
}
