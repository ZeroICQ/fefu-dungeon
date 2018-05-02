#include <vector>
#include <random>
#include "actors.h"

using std::vector;
using std::shared_ptr;

game::ActorFactory::ActorFactory()
{
    add_actor<Wall>();
    add_actor<MainCharActor>();
    add_actor<EmptyActor>();
    add_actor<GuardActor>();
    add_actor<Target>();
}

game::FloorActorFactory::FloorActorFactory()
{
    add_actor<EmptyFloor>();
}


void game::MainCharActor::move(game::GameControls controls, shared_ptr<game::Map> map)
{
    auto desired_row = row();
    auto desired_col = col();

    switch (controls) {
        case GameControls::move_up:
            desired_row -= 1;
            break;
        case GameControls::move_right:
            desired_col += 1;
            break;
        case GameControls::move_down:
            desired_row += 1;
                break;
        case GameControls::move_left:
            desired_col -= 1;
            break;
        default:
            break;
    }

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

void game::MainCharActor::collide(game::ActiveActor& other, const shared_ptr<game::Map> map)
{
    if (!other.is_enemy()) {
        return;
    }

    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), other.attack_damage());
}


void game::EmptyActor::collide(game::ActiveActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_move(other.get_ptr(), row_, col_);
}

void game::GuardActor::move(game::GameControls controls, const shared_ptr<game::Map> map) {
    enum Directions {up, right, down, left, count};
    //TODO: move? to map as "find_main_character_near"
    //if player is near hit him

    bool is_player_near = false;
    //TODO: refactor
    if (map->is_inbound(row()-1, col()) && !map->get_cell(row()-1, col())->actor()->is_enemy()) {
        map->get_cell(row()-1, col())->actor()->collide(*this, map);
        is_player_near = true;
    } else if (map->is_inbound(row()+1, col()) && !map->get_cell(row()+1, col())->actor()->is_enemy()) {
        map->get_cell(row()+1, col())->actor()->collide(*this, map);
        is_player_near = true;
    } else if (map->is_inbound(row(), col()-1) && !map->get_cell(row(), col()-1)->actor()->is_enemy()) {
        map->get_cell(row(), col()-1)->actor()->collide(*this, map);
        is_player_near = true;
    } else if (map->is_inbound(row(), col()+1) && !map->get_cell(row(), col()+1)->actor()->is_enemy()) {
        map->get_cell(row(), col()+1)->actor()->collide(*this, map);
        is_player_near = true;
    }

    if (is_player_near) {
        return;
    }

    int rnd_direction = rand() % (count);

    int desired_row = row();
    int desired_col = col();

    switch (rnd_direction) {
        case Directions::up:
            desired_row -= 1;
            break;
        case Directions::right:
            desired_col += 1;
            break;
        case Directions::down:
            desired_row += 1;
            break;
        case Directions::left:
            desired_col -= 1;
            break;
        default:
            break;
    }

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

shared_ptr<game::Actor> game::Actor::get_ptr()
{
    return shared_from_this();
}

//ASK: is it cancer?
void game::Actor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    this->collide(*static_cast<ActiveActor*>(&other), map);
}

//ASK: полиморфизм. Перегрузка всех функций в одну в наследнике. Избавиться от флагов (надо ли)?
void game::EnemyActor::collide(game::ActiveActor &other, const shared_ptr<game::Map> map)
{
    if (other.is_enemy()) {
        return;
    }

    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), other.attack_damage());
}

void game::Target::collide(game::MainCharActor &other, const shared_ptr<game::Map> map)
{
    //TODO: you win
}
