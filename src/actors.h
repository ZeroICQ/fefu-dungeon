#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "statuses.h"
#include "map.h"
#include "event_system.h"
#include "colors.h"

namespace game {

//fd circular dependency
class Map;
class MapCell;
class MainCharActor;
class EmptyActor;
class ActiveActor;

class Actor: public std::enable_shared_from_this<Actor>
{
public:
    explicit Actor(int row = 0, int col = 0, char icon = '-', int max_hp = 100,
                   int attack_damage = 0, short color_pair = Colors::DEFAULT)
            : row_(row), col_(col), map_icon_(icon), max_hp_(max_hp), curr_hp_(max_hp),
              attack_damage_{attack_damage}, color_pair_(color_pair) {}

    std::shared_ptr<Actor> get_ptr();

    char map_icon() const { return map_icon_; }

    int row() const { return row_; }
    void row(int replace) { row_ = replace; }
    int col() const { return col_; }
    void col(int replace) { col_  = replace; }

    void set_pos(int r_row, int r_col) { row(r_row); col(r_col); }

    virtual void move(GameControls controls, const std::shared_ptr<Map> map) {}

    virtual void collide(Actor& other, const std::shared_ptr<Map> map) = 0;
    virtual void collide(ActiveActor& other, const std::shared_ptr<Map> map) {}

    //flags
    virtual bool is_transparent() const { return false; }
    virtual bool is_playable() const { return false; }
    virtual bool is_enemy() const { return true; }

    //getters and setters
    int max_hp() const { return max_hp_; }
    void max_hp(int max_hp) { max_hp_ = max_hp; }

    int curr_hp() const { return curr_hp_; }
    void curr_hp(int curr_hp) { curr_hp_ = curr_hp; }

    int attack_damage() { return attack_damage_; }

    short color_pair() const { return color_pair_; }

protected:
    int row_;
    int col_;
    char map_icon_;
    int max_hp_;
    int curr_hp_;
    int attack_damage_;
    short color_pair_;
};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int row = 0, int col = 0, char icon = 'F') : Actor(row, col, icon) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class EmptyFloor : public FloorActor
{
public:
    explicit EmptyFloor(int row = 0, int col = 0, char icon = ' ') : FloorActor(row, col, icon) {}
    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class EmptyActor : public Actor
{
public:
    explicit EmptyActor(int row = 0, int col = 0, char icon = ' ') : Actor(row, col, icon) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ActiveActor& other, const std::shared_ptr<Map> map) override;

    bool is_transparent() const final { return true; }
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#') : Actor(row, col, icon, 100, 0, Colors::FULL_WHITE) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }

};


class ActiveActor : public Actor
{
public:
    explicit ActiveActor(int row = 0, int col = 0, char icon = 'A', int hit_points = 0, int attack_damage = 0)
            :  Actor(row, col, icon, hit_points, attack_damage) {}
    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }

};


class MainCharActor : public ActiveActor
{
public:
    explicit MainCharActor(int row = 0, int col = 0, char icon ='S', int hit_points = 500, int attack_damage = 100)
            : ActiveActor(row, col, icon, hit_points, attack_damage) {}

    void move(GameControls controls, std::shared_ptr<Map> map) override;

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ActiveActor& other, const std::shared_ptr<Map> map) override;

    //flags
    bool is_playable() const override { return true; }
    bool is_enemy() const override { return false; }
};


class EnemyActor : public ActiveActor
{
public:
    explicit EnemyActor(int row = 0, int col = 0, char icon = 'E', int hit_points = 0,
            int attack_damage = 100)
            : ActiveActor(row, col, icon, hit_points, attack_damage) {}

    void collide(Actor &other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class GuardActor : public EnemyActor
{
public:
    explicit GuardActor(int row = 0, int col = 0, char icon ='G', int hit_points = 50, int attack_damage = 100)
            : EnemyActor(row, col, icon, hit_points, attack_damage) {}

    void move(GameControls controls, const std::shared_ptr<Map> map) override;
//    void move(GameControls control, Map& map) override;

//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};


template <class BaseT>
class BaseFactory
{
public:
    template <class ActorT>
    void add_actor()
    {
        constructors_[ActorT().map_icon()] = [](int row, int col){return new ActorT(row, col);};
    }

    //CAN return nullptr
    std::shared_ptr<BaseT> create(char icon, int row, int col)
    {
        if (!constructors_.count(icon)) {
            return std::shared_ptr<BaseT>(nullptr);
        }
        return std::shared_ptr<BaseT>(constructors_[icon](row, col));
    }

private:
    std::map<char, std::function<BaseT*(int, int)>> constructors_;
};


class ActorFactory : public BaseFactory<Actor>
{
public:
    explicit ActorFactory();
};


class FloorActorFactory : public BaseFactory<FloorActor>
{
public:
    explicit FloorActorFactory();
};

} //namespace game