#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "statuses.h"

namespace game {

class Actor
{
public:
    explicit Actor(int row = 0, int col = 0, char icon = '-') : row_(row), col_(col), map_icon_(icon) {}

    virtual void move(GameControls control) {};

    char map_icon() const { return map_icon_; }
    int row() const { return row_; }
    int col() const { return col_; }

protected:
    int row_;
    int col_;
    char map_icon_;
};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int row = 0, int col = 0, char icon = 'F') : Actor(row, col, icon) {}
};


class EmptyFloor : public FloorActor
{
public:
    explicit EmptyFloor(int row = 0, int col = 0, char icon = ' ') : FloorActor(row, col, icon) {}
};


class EmptyActor : public Actor
{
public:
    explicit EmptyActor(int row = 0, int col = 0, char icon = ' ') : Actor(row, col, icon) {}
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#') : Actor(row, col, icon) {}
};


class ActiveActor : public Actor
{
public:
    explicit ActiveActor(int row = 0, int col = 0, char icon = 'A') :  Actor(row, col, icon) {}
    void move_up();
    void move_right();
    void move_down();
    void move_left();
//    virtual bool can_move(int col, int row);
};


class MainCharActor : public ActiveActor
{
public:
    void move(GameControls control) override;
    explicit MainCharActor(int row = 0, int col = 0, char icon ='S') : ActiveActor(row, col, icon) {}
};


template <class BaseT>
class BaseFactory
{
public:
    template <class ActorT>
    void add_actor();
    //CAN return nullptr
    std::shared_ptr<BaseT> create(char icon, int row, int col);

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