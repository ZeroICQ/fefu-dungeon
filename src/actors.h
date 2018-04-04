#pragma once

#include <string>
#include <functional>
#include <map>
#include "game.h"

namespace game {

class Actor
{
public:
    Actor() : Actor(0, 0) {}
    Actor(int y, int x, char icon = '-') : y_(y), x_(x), map_icon_(icon) {}

    virtual void move(GameControls control) = 0;
    virtual void move() = 0;

    char map_icon() const { return map_icon_; }
    virtual bool is_playable() const { return false; }
protected:
    int y_;
    int x_;
    char map_icon_;
};

class FloorActor : public Actor
{
public:
    FloorActor() : FloorActor(0, 0) {}
    FloorActor(int y, int x, char icon = 'F') : Actor(y, x, icon) {}

    void move(GameControls control) override;
};


class EmptyFloor: public  FloorActor
{
public:
    EmptyFloor() : EmptyFloor(0, 0) {}
    EmptyFloor(int y, int x) : FloorActor(y, x, ' ') {}
};

//class LavaFloor: public FloorActor
//{
//public:
//    Floo
//};

class VoidActor: public Actor
{
public:
    //ASK: нельзя вынести?
    VoidActor() : VoidActor(0, 0) {}
    VoidActor(int y, int x) : Actor(y, x, ' ') {}
};

class Wall : public Actor
{
public:
    Wall() : Wall(0,0) {}
    Wall(int y, int x) : Actor(y, x, '#') {}

};

class MainCharActor : public Actor
{
public:
    MainCharActor() : MainCharActor(0, 0) {}
    MainCharActor(int y, int x) : Actor(y, x, 'S') {}

    bool is_playable() const override { return true; }
};

template <class T>
class Factory
{
public:
    template <class G>
    bool add_actor(G actor, char icon);
    //can return nullptr
    T* create(char icon, int y, int x);
private:
    std::map<char, std::function<T*(int, int)>> constructors_;
};


class ActorFactory
{
public:
    ActorFactory();
    Actor* create(char icon, int y, int x);

private:
    Factory<Actor> real_factory_;
};


class FloorActorFactory
{
public:
    FloorActorFactory();
    FloorActor* create(char icon, int y, int x);
private:
    Factory<FloorActor> real_factory_;
};

} //namespace game