#pragma once

#include <string>
#include <functional>
#include <map>

namespace game {

class Actor
{
public:
    Actor() : Actor(0, 0) {}
    Actor(int y, int x, char icon = ' ') : y_(y), x_(x), map_icon_(icon) {}

    char map_icon() const { return map_icon_; };
protected:
    int y_;
    int x_;
    char map_icon_;
};

class Floor : public Actor
{
public:
    Floor() : Floor(0, 0) {}
    Floor(int y, int x) : Actor(y, x, ' ') {}
protected:
    const char map_icon_ = '=';
};

class Wall : public Actor
{
public:
    Wall() : Wall(0,0) {}
    Wall(int y, int x) : Actor(y, x, '#') {}

protected:
};

class ActorFactory
{
public:
    ActorFactory();
    template<class T>
    bool add_actor(T actor, char icon);
    Actor* create(char icon, int y, int x);
private:
    std::map<char, std::function<Actor*(int, int)>> constructors_;
};

} //namespace game