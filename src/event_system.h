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
    void move_projectiles(Game& game, std::shared_ptr<Map> map, GameControls control,
                          const std::chrono::time_point<std::chrono::steady_clock>& curr_tp);
    void spawn_projectiles(Game& game, std::shared_ptr<Map> map);

    void manage_projectile(std::shared_ptr<ProjectileActor> projectile);

    void add_projectile(std::shared_ptr<ProjectileActor> projectile);
    void add_move(std::shared_ptr<Actor> actor, int row_to, int col_to);
    void add_damage(std::shared_ptr<Actor> from, std::shared_ptr<Actor> to, int damage);
    void add_target_reached();
    void add_heal(std::shared_ptr<Actor> to, int restore);
    void add_mana(std::shared_ptr<Actor> to, int restore);

private:
    EventManager();
//    ~EventManager();
    std::deque<std::shared_ptr<ProjectileActor>> projectiles_;

    std::shared_ptr<std::deque<std::shared_ptr<Event>>> spawn_projectile_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> move_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> damage_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> heal_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> target_reached_event_pool_;
    std::shared_ptr<std::deque<std::shared_ptr<Event>>> mana_restore_event_pool_;

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

class HealEvent : public Event
{
public:
    HealEvent(std::shared_ptr<Actor> to, int restore)
    : actor_to_(to), restore_(restore) {}

    void trigger(Game &game, std::shared_ptr<Map> map) override;

private:
    std::shared_ptr<Actor> actor_to_;
    int restore_;
};

class ManaRestoreEvent : public Event
{
public:
    ManaRestoreEvent(std::shared_ptr<Actor> to, int restore)
    : actor_to_(to), restore_(restore) {}

    void trigger(Game& game, std::shared_ptr<Map> map) override;
private:
    std::shared_ptr<Actor> actor_to_;
    int restore_;
};

class SpawnProjectileEvent : public Event
{
public:
    SpawnProjectileEvent(std::shared_ptr<ProjectileActor> projectile, int row, int col)
            : projectile_(projectile), row_(row), col_(col) {}

    void trigger(Game &game, std::shared_ptr<Map> map) override;

private:
    std::shared_ptr<ProjectileActor> projectile_;
    int row_;
    int col_;
};


} //namespace game
