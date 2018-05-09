#include "event_system.h"
#include "statuses.h"

game::EventManager& game::EventManager::instance()
{
    static EventManager e;
    return e;
}

game::EventManager::EventManager()
{
    spawn_projectile_event_pool_ = std::make_shared<std::deque<std::shared_ptr<Event>>>();


    move_event_pool_ = std::make_shared<std::deque<std::shared_ptr<Event>>>();
    heal_event_pool_= std::make_shared<std::deque<std::shared_ptr<Event>>>();
    mana_restore_event_pool_= std::make_shared<std::deque<std::shared_ptr<Event>>>();
    damage_event_pool_ = std::make_shared<std::deque<std::shared_ptr<Event>>>();
    target_reached_event_pool_= std::make_shared<std::deque<std::shared_ptr<Event>>>();

    event_pools_.push_back(heal_event_pool_);
    event_pools_.push_back(mana_restore_event_pool_);
    event_pools_.push_back(damage_event_pool_);
    event_pools_.push_back(move_event_pool_);
    event_pools_.push_back(target_reached_event_pool_);
}

void game::EventManager::trigger_all(Game& game, std::shared_ptr<Map> map)
{
    for (auto& event_pool : event_pools_) {
        while (!event_pool->empty()) {
            event_pool->front()->trigger(game, map);
            event_pool->pop_front();
        }
    }
}

void game::EventManager::add_move(std::shared_ptr<game::Actor> actor, int row_to, int col_to)
{
    move_event_pool_->emplace_back(new MoveEvent(actor, row_to, col_to));
}

void game::EventManager::add_damage(std::shared_ptr<game::Actor> from, std::shared_ptr<game::Actor> to, int damage) {
    damage_event_pool_->emplace_back(new DamageEvent(from, to, damage));
}

void game::EventManager::add_target_reached()
{
    target_reached_event_pool_->emplace_back(new TargetReachedEvent());
}

void game::EventManager::add_heal(std::shared_ptr<game::Actor> to, int restore)
{
    heal_event_pool_->emplace_back(new HealEvent(to, restore));
}

void game::EventManager::add_projectile(std::shared_ptr<game::ProjectileActor> projectile)
{
    spawn_projectile_event_pool_->emplace_back(
            new SpawnProjectileEvent(projectile, projectile->row(), projectile->col()));
}

void game::EventManager::move_projectiles(game::Game& game, std::shared_ptr<game::Map> map, GameControls control)
{
    for (auto i = projectiles_.begin(); i != projectiles_.end();) {
        i->get()->move(control, map);
        i->get()->is_made_turn(true);
        trigger_all(game, map);

        //don't forget to remove from map
        if (i->get()->is_dead()) {
            i = projectiles_.erase(i);
        } else {
            i++;
        }
    }
}

void game::EventManager::spawn_projectiles(Game& game, std::shared_ptr<Map> map)
{
    while (!spawn_projectile_event_pool_->empty()) {
        spawn_projectile_event_pool_->front()->trigger(game, map);
        spawn_projectile_event_pool_->pop_front();
        trigger_all(game, map);
    }
}

void game::EventManager::manage_projectile(std::shared_ptr<game::ProjectileActor> projectile)
{
    projectiles_.push_back(projectile);
}

void game::EventManager::add_mana(std::shared_ptr<game::Actor> to, int restore)
{
    mana_restore_event_pool_->emplace_back(new ManaRestoreEvent(to, restore));
}

void game::MoveEvent::trigger(Game& game, std::shared_ptr<Map> map)
{
    if (map->is_inbound(row_to_, col_to_) &&
        (map->get_cell(row_to_, col_to_)->actor()->is_transparent() || map->get_cell(row_to_, col_to_)->actor()->is_dead())) {
        map->move_actor(actor_->row(), actor_->col(), row_to_, col_to_);
    }
}

void game::DamageEvent::trigger(Game& game, std::shared_ptr<game::Map> map) {
    actor_to_->hit(actor_from_->attack_damage());
}

void game::TargetReachedEvent::trigger(Game& game, std::shared_ptr<game::Map> map)
{
    game.reach_target();
}

void game::HealEvent::trigger(game::Game& game, std::shared_ptr<game::Map> map)
{
    actor_to_->heal(restore_);
}

void game::SpawnProjectileEvent::trigger(game::Game& game, std::shared_ptr<game::Map> map)
{
    map->get_cell(projectile_->row(), projectile_->col())->actor()->collide(*projectile_, map);

    EventManager::instance().trigger_all(game, map);
    if (!projectile_->is_dead()) {
        map->replace_actor(projectile_->row(), projectile_->col(), projectile_);
        EventManager::instance().manage_projectile(projectile_);
    }
}

void game::ManaRestoreEvent::trigger(game::Game& game, std::shared_ptr<game::Map> map)
{
    actor_to_->restore_mana(restore_);
}
