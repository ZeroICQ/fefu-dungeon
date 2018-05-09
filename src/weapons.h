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


class Fireball : public Weapon
{
public:
    explicit Fireball(int mana_cost = 100, std::string name = "Fireball") : Weapon(mana_cost, name) {}

    void shoot(int row, int col, Directions direction) override;

};

}//namespace game
