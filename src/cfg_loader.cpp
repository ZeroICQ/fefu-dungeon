#include <string>
#include <libconfig.h++>
#include <vector>
#include "cfg_loader.h"

using std::string;
using std::vector;
using libconfig::Setting;
using libconfig::SettingNotFoundException;

game::Cfg &game::Cfg::instance()
{
    static Cfg s;
    return s;
}

game::Cfg::Cfg(string config_path)
{
    vector<string> actors = {"MainCharactor"};

    cfg_.readFile(config_path.c_str());

    Setting& root = cfg_.getRoot();

    for (const auto& actor_cfg_name : actors) {
        const auto& curr_root = root[actor_cfg_name];

        double speed;
        char icon;
        int max_hp;
        int attack_damage;
        short color_pair;
        int max_mana;

        try {
            speed = curr_root.lookup("speed");
        } catch(const SettingNotFoundException &e) {
            speed = 0.0;
        }

        try {
            icon = curr_root.lookup("icon").c_str()[0];
        } catch(const SettingNotFoundException &e) {
            icon = '-';
        }

        try {
            max_hp = curr_root.lookup("hp");
        } catch(const SettingNotFoundException &e) {
            max_hp = 1;
        }

        try {
            attack_damage= curr_root.lookup("attack");
        } catch(const SettingNotFoundException &e) {
            attack_damage = 1;
        }

        try {
            color_pair = static_cast<short>(static_cast<int>(curr_root.lookup("color")));
        } catch(const SettingNotFoundException &e) {
            color_pair = 0;
        }

        try {
            max_mana = curr_root.lookup("mana");
        } catch(const SettingNotFoundException &e) {
            max_mana = 1;
        }


        actor_settings_.emplace_back(speed, icon, max_hp, attack_damage, color_pair, max_mana);
    }
}


//
//struct ActorSettings
//{
//    double speed;
//    char icon;
//    int max_hp;
//    int attack_damage;
//    short color_pair;
//    int max_mana;
//};
