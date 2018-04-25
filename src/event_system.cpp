#include "event_system.h"

game::EventManager& game::EventManager::instance()
{
    static EventManager e;
    return e;
}

game::EventManager::EventManager()
{
    move_event_pool_ = std::make_shared<std::list<std::shared_ptr<Event>>>();
    damage_event_pool_ = std::make_shared<std::list<std::shared_ptr<Event>>>();

    event_pools_.push_back(move_event_pool_);
    event_pools_.push_back(damage_event_pool_);
}

void game::EventManager::trigger_all(std::shared_ptr<Map> map)
{
    for (auto& event_pool : event_pools_) {
        auto event = event_pool->begin();

        while (event != event_pool->end()) {
            event->get()->trigger(map);
            event = event_pool->erase(event);
        }
    }
}

void game::EventManager::add_move(std::shared_ptr<game::Actor> actor, int row_to, int col_to)
{
    move_event_pool_->emplace_back(new MoveEvent(actor, row_to, col_to));
}

void game::MoveEvent::trigger(std::shared_ptr<Map> map)
{
    if (map->is_inbound(row_to_, col_to_) && map->get_cell(row_to_, col_to_)->actor()->is_transparent()) {
        map->move_actor(actor_->row(), actor_->col(), row_to_, col_to_);
    }
}
