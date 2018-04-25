#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "statuses.h"
#include "map.h"

namespace game {

//predeclare circular dependency
class Map;
class MainCharActor;
class EmptyActor;
class ActiveActor;

class Actor
{
public:
    explicit Actor(int row = 0, int col = 0, char icon = '-') : row_(row), col_(col), map_icon_(icon) {}

//    virtual void move(GameControls control, Map& map) {};

    char map_icon() const { return map_icon_; }
    int row() const { return row_; }
    void row(int replace) { row_ = replace; }
    int col() const { return col_; }
    void col(int replace) { col_  = replace; }

    void set_pos(int r_row, int r_col) { row(r_row); col(r_col); }

//    virtual void collide(Actor& other, Map& map) = 0;
//    virtual void collide(MainCharActor& other, Map& map) {}

protected:
    int row_;
    int col_;
    char map_icon_;
};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int row = 0, int col = 0, char icon = 'F') : Actor(row, col, icon) {}
//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};


class EmptyFloor : public FloorActor
{
public:
    explicit EmptyFloor(int row = 0, int col = 0, char icon = ' ') : FloorActor(row, col, icon) {}
//    void collide`(Actor& other, Map& map) override { return other.collide(*this, map); }
};


class EmptyActor : public Actor
{
public:
    explicit EmptyActor(int row = 0, int col = 0, char icon = ' ') : Actor(row, col, icon) {}
//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#') : Actor(row, col, icon) {}
//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }

};


class ActiveActor : public Actor
{
public:
    explicit ActiveActor(int row = 0, int col = 0, char icon = 'A') :  Actor(row, col, icon) {}
//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }

};


class MainCharActor : public ActiveActor
{
public:
    explicit MainCharActor(int row = 0, int col = 0, char icon ='S') : ActiveActor(row, col, icon) {}
//    void move(GameControls controls, Map& map) override;
//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};


class EnemyActor : public ActiveActor
{
public:
    explicit EnemyActor(int row = 0, int col = 0, char icon = 'E') : ActiveActor(row, col, icon) {}
};


class GuardActor : public EnemyActor
{
public:
    explicit GuardActor(int row = 0, int col = 0, char icon ='G') : EnemyActor(row, col, icon) {}

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