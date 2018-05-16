#pragma once

#include <string>
#include <vector>
#include <libconfig.h++>
#include "cfg_loader.h"
#include "actor_settings.h"

namespace game {

enum ActorCfg {MAINCHAR, GUARD, TEACHER, FIREBALL, STONE};
enum PotionCfg {HP, MANA};

class Cfg
{
public:
    static Cfg& instance();
    static const ActorSettings& getMainChar() { return Cfg::instance().get_actor_settings(ActorCfg::MAINCHAR); }
    static const ActorSettings& getGuard() { return Cfg::instance().get_actor_settings(ActorCfg::GUARD); }
    static const ActorSettings& getTeacher() { return Cfg::instance().get_actor_settings(ActorCfg::TEACHER); }
    static const ActorSettings& getFireball() { return Cfg::instance().get_actor_settings(ActorCfg::FIREBALL); }
    static const ActorSettings& getStone() { return Cfg::instance().get_actor_settings(ActorCfg::STONE); }

    static const PotionSettings& getHp() { return Cfg::instance().get_potion_settings(PotionCfg::HP); }
    static const PotionSettings& getMana() { return Cfg::instance().get_potion_settings(PotionCfg::MANA); }

    const ActorSettings& get_actor_settings(ActorCfg actor_index) const { return actor_settings_[actor_index]; }
    const PotionSettings& get_potion_settings(PotionCfg potion_index) const { return potion_settings_[potion_index]; }
private:
    libconfig::Config cfg_;
    explicit Cfg(std::string config_path = "settings.cfg");

    std::vector<ActorSettings> actor_settings_;
    std::vector<PotionSettings> potion_settings_;
};

}//namespace game

