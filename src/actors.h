#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>

namespace game {

class Actor
{
public:
    explicit Actor(int y = 0, int x = 0, char icon = '-') : y_(y), x_(x), map_icon_(icon) {}

//    virtual void move(GameControls control) = 0;
//    virtual void move() = 0;

    char map_icon() const { return map_icon_; }
protected:
    int y_;
    int x_;
    char map_icon_;
};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int y = 0, int x = 0, char icon = 'F') : Actor(y, x, icon) {}
};


class EmptyFloor: public  FloorActor
{
public:
    explicit EmptyFloor(int y = 0, int x = 0) : FloorActor(y, x, ' ') {}
};


class EmptyActor: public Actor
{
public:
    explicit EmptyActor(int y = 0, int x = 0) : Actor(y, x, ' ') {}
};


class Wall : public Actor
{
public:
    explicit Wall(int y = 0, int x = 0) : Actor(y, x, '#') {}

};


class MainCharActor : public Actor
{
public:
    explicit MainCharActor(int y = 0, int x = 0) : Actor(y, x, 'S') {}
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
    ActorFactory();
};


class FloorActorFactory : public BaseFactory<FloorActor>
{
public:
    FloorActorFactory();
};

} //namespace game