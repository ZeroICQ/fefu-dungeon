#pragma once

#include <deque>
#include "actors.h"
#include "game.h"

namespace game {

class Game;
class Map;
class Actor;
class Event;

//Dont forget to register event pools in consturctor
class EventManager
{
public:
    //Meyer's singleton
    static EventManager& instance();
    void trigger_all(Game& game, std::shared_ptr<Map> map);

    void add_move(std::shared_ptr<Actor> actor, int row_to, int col_to);
    void add_damage(std::shared_ptr<Actor> from, std::shared_ptr<Actor> to, int damage);
    void add_target_reached();

private:
    EventManager();
//    ~EventManager();
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> move_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> damage_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> target_reached_event_pool_;

    std::deque<std::shared_ptr<std::deque<std::shared_ptr<Event>>>> event_pools_;
};


class Event
{
public:
    virtual void trigger(Game& game, std::shared_ptr<Map> map) = 0;

};


class MoveEvent : public Event
{
public:
    MoveEvent(std::shared_ptr<Actor> actor, int row_to, int col_to)
            : actor_(actor), row_to_(row_to), col_to_(col_to) {}

    void trigger(Game& game, std::shared_ptr<Map> map) override;

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

    void trigger(Game& game, std::shared_ptr<Map> map) override;

private:
    std::shared_ptr<Actor> actor_from_;
    std::shared_ptr<Actor> actor_to_;
    int damage_;
};

class TargetReachedEvent : public Event
{
public:
    void trigger(Game& game, std::shared_ptr<Map> map) override;
};

} //namespace game
