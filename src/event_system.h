#pragma once

#include <list>
#include <vector>
#include "actors.h"

namespace game {

class Map;
class Actor;
class Event;

class EventManager
{
public:
    //Meyer's singleton
    static EventManager& instance();
    void trigger_all(std::shared_ptr<Map> map);

    void add_move(std::shared_ptr<Actor> actor, int row_to, int col_to);
    void add_damage(std::shared_ptr<Actor> from, std::shared_ptr<Actor> to, int damage);

private:
    EventManager();
//    ~EventManager();

    //ASK: вектор шаблонных листов наследованного типа.
    std::shared_ptr<std::list<std::shared_ptr<Event>>> move_event_pool_;
    std::shared_ptr<std::list<std::shared_ptr<Event>>> damage_event_pool_;

    std::vector<std::shared_ptr<std::list<std::shared_ptr<Event>>>> event_pools_;
};


class Event
{
public:
    virtual void trigger(std::shared_ptr<Map> map) = 0;

};


class MoveEvent : public Event
{
public:
    MoveEvent(std::shared_ptr<Actor> actor, int row_to, int col_to)
            : actor_(actor), row_to_(row_to), col_to_(col_to) {}

    void trigger(std::shared_ptr<Map> map) override;

private:
    std::shared_ptr<Actor> actor_;
    int row_to_;
    int col_to_;
};


class DamageEvent : public Event
{
public:
    DamageEvent(std::shared_ptr<Actor> from, std::shared_ptr<Actor> to, int damage)
            : actor_from_(from), actor_to_(to), damage_(damage) {}

    void trigger(std::shared_ptr<Map> map) override;

private:
    std::shared_ptr<Actor> actor_from_;
    std::shared_ptr<Actor> actor_to_;
    int damage_;
};

} //namespace game
