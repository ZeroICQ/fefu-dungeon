#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "statuses.h"
#include "map.h"
#include "event_system.h"

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
    explicit Actor(int row = 0, int col = 0, char icon = '-', int hit_points = 100,
                   int attack_damage = 0)
            : row_(row), col_(col), map_icon_(icon), hit_points_(hit_points),
              attack_damage_{attack_damage} {}

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
    virtual bool is_transparent() { return false; }
    virtual bool is_playable() { return false; }

    int hp() { return hit_points_; }
    void hp(int s_hp) { hit_points_ = s_hp; }
//  Possible underflow bug
    void hit(int dmg) { hit_points_ -= dmg; }

protected:
    int row_;
    int col_;
    char map_icon_;
    int hit_points_;
    int attack_damage_;
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

    bool is_transparent() final { return true; }
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#') : Actor(row, col, icon) {}

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

    //flags
    bool is_playable() override { return true; }
};


class EnemyActor : public ActiveActor
{
public:
    explicit EnemyActor(int row = 0, int col = 0, char icon = 'E', int hit_points = 0)
            : ActiveActor(row, col, icon, hit_points) {}

    void collide(Actor &other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class GuardActor : public EnemyActor
{
public:
    explicit GuardActor(int row = 0, int col = 0, char icon ='G', int hit_points = 50)
            : EnemyActor(row, col, icon, hit_points) {}

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