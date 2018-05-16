#pragma once

struct ActorSettings
{
    ActorSettings(double speed, char icon, int max_hp, int attack_damage, short color_pair, int max_mana)
            : speed(speed), icon(icon), max_hp(max_hp), attack_damage(attack_damage), color_pair(color_pair),
              max_mana(max_mana) {}

    double speed;
    char icon;
    int max_hp;
    int attack_damage;
    short color_pair;
    int max_mana;
};
