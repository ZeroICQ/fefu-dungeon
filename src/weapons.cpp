#include "weapons.h"
#include "event_system.h"
#include "actors.h"

void game::FireballWeapon::shoot(int row, int col, game::Directions direction)
{
    Actor::direction_to_coord(direction, row, col);
    EventManager::instance().add_projectile(std::make_shared<FireballActor>(row, col, direction));
}

void game::StoneWeapon::shoot(int row, int col, game::Directions direction)
{
    Actor::direction_to_coord(direction, row, col);
    EventManager::instance().add_projectile(std::make_shared<StoneActor>(row, col, direction));
}
