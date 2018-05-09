#pragma once

#include "actors.h"

namespace game {

enum class Directions;

class Weapon
{
public:
    explicit Weapon(int mana_cost = 100) : mana_cost_(mana_cost) {}
    virtual void shoot(int row, int col, Directions direction) = 0;

    int mana_cost() const { return mana_cost_; }
private:
    int mana_cost_;
};


class SingleShot : public Weapon
{
public:
    explicit SingleShot(int mana_cost = 100) : Weapon(mana_cost) {}

    void shoot(int row, int col, Directions direction) override;

};

}//namespace game
