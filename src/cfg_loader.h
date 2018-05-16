#pragma once

#include <string>
#include <vector>
#include <libconfig.h++>
#include "cfg_loader.h"
#include "actor_settings.h"

namespace game {

enum ActorCfg {MAINCHAR, GUARD, TEACHER, FIREBALL, STONE};

class Cfg
{
public:
    static Cfg& instance();
    static const ActorSettings& getMainChar() { return Cfg::instance().get_settings(ActorCfg::MAINCHAR); }
    static const ActorSettings& getGuard() { return Cfg::instance().get_settings(ActorCfg::GUARD); }
    static const ActorSettings& getTeacher() { return Cfg::instance().get_settings(ActorCfg::TEACHER); }
    static const ActorSettings& getFireball() { return Cfg::instance().get_settings(ActorCfg::FIREBALL); }
    static const ActorSettings& getStone() { return Cfg::instance().get_settings(ActorCfg::STONE); }

    const ActorSettings& get_settings(ActorCfg actor_index) const { return actor_settings_[actor_index]; }
private:
    libconfig::Config cfg_;
    explicit Cfg(std::string config_path = "settings.cfg");

    std::vector<ActorSettings> actor_settings_;
};

}//namespace game

