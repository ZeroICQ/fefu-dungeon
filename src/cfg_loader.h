#pragma once

#include <string>
#include <vector>
#include <libconfig.h++>
#include "cfg_loader.h"
#include "actor_settings.h"

namespace game {

enum ActorCfg {MainCharactor};

class Cfg
{
public:
    static Cfg& instance();
    static const ActorSettings& getMainChar() { return Cfg::instance().get_settings(ActorCfg::MainCharactor); }

    const ActorSettings& get_settings(ActorCfg actor_index) const { return actor_settings_[actor_index]; }
private:
    libconfig::Config cfg_;
    explicit Cfg(std::string config_path = "settings.cfg");

    std::vector<ActorSettings> actor_settings_;
};

}//namespace game

