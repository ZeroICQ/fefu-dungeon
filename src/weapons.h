#pragma once

#include <utility>
#include "actors.h"

namespace game {

enum class Directions;

class Weapon
{
public:
    explicit Weapon(int mana_cost = 100, std::string name = "Weapon") : mana_cost_(mana_cost), name_(std::move(name)) {}
    virtual void shoot(int row, int col, Directions direction) = 0;

    int mana_cost() const { return mana_cost_; }
    std::string name() const { return name_; }

private:
    int mana_cost_;
    std::string name_;
};


class FireballWeapon : public Weapon
{
public:
    explicit FireballWeapon(int mana_cost = 100, std::string name = "FireballWeapon") : Weapon(mana_cost, name) {}

    void shoot(int row, int col, Directions direction) override;

};

class StoneWeapon : public Weapon
{
public:
    explicit StoneWeapon(int mana_cost = 150, std::string name = "Stone") : Weapon(mana_cost, name) {}

    void shoot(int row, int col, Directions direction) override;
};

}//namespace game
