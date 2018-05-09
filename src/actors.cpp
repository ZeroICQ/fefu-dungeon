#include <vector>
#include <random>
#include <termcap.h>
#include "actors.h"
#include "event_system.h"

using std::vector;
using std::shared_ptr;

game::ActorFactory::ActorFactory()
{
    add_actor<Wall>();
    add_actor<MainCharActor>();
    add_actor<EmptyActor>();
    add_actor<GuardActor>();
    add_actor<TargetActor>();
    add_actor<TeacherActor>();
    add_actor<HealPotionActor>();
    add_actor<ManaPotionActor>();
    add_actor<FireballActor>();
}

game::FloorActorFactory::FloorActorFactory()
{
    add_actor<EmptyFloor>();
}


void game::MainCharActor::move(game::GameControls controls, shared_ptr<game::Map> map)
{
    auto desired_row = row();
    auto desired_col = col();

    if (controls == GameControls::shoot) {
        shoot();
        return;
    }

    switch (controls) {
        case GameControls::move_up:
            direction(Directions::UP);
            break;
        case GameControls::move_right:
            direction(Directions::RIGHT);
            break;
        case GameControls::move_down:
            direction(Directions::DOWN);
                break;
        case GameControls::move_left:
            direction(Directions::LEFT);
            break;
        default:
            break;
    }

    direction_to_coord(direction(), desired_row, desired_col);

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

void game::MainCharActor::collide(game::EnemyActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), other.attack_damage());
}

void game::MainCharActor::shoot()
{
    if (curr_mana() - weapon()->mana_cost() >= 0)
    {
        decrease_mana(weapon()->mana_cost());
        ActiveActor::shoot();
    }
}


void game::EmptyActor::collide(game::ActiveActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_move(other.get_ptr(), row_, col_);
}

void game::GuardActor::move(game::GameControls controls, const shared_ptr<game::Map> map) {
    MapSearchResult player_search = map->find_player_near(row(), col());

    if (player_search.is_found) {
        map->get_cell(player_search.row, player_search.col)->actor()->collide(*this, map);
        direction(coord_to_direction(row(), col(), player_search.row, player_search.col));
        return;
    }

    direction(RndHelper::rand_direction());

    int desired_row = row();
    int desired_col = col();

    direction_to_coord(direction(), desired_row, desired_col);

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    //20% chance shooting
    //dont shoot and go same time
    if (RndHelper::rand_yes_no(0.2) == YesOrNo::YES) {
        shoot();
        return;
    }


    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

shared_ptr<game::Actor> game::Actor::get_ptr()
{
    return shared_from_this();
}

void game::Actor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    this->collide(*static_cast<ActiveActor*>(&other), map);
}

void game::Actor::collide(game::EnemyActor &other, const shared_ptr<game::Map> map)
{
    this->collide(*static_cast<ActiveActor*>(&other), map);
}

void game::Actor::direction_to_coord(game::Directions direction, int &row, int &col)
{
    switch (direction) {
        case Directions::UP:
            row -= 1;
            break;
        case Directions::RIGHT:
            col += 1;
            break;
        case Directions::DOWN:
            row += 1;
            break;
        case Directions::LEFT:
            col -= 1;
            break;
    }
}

game::Directions game::Actor::coord_to_direction(int s_row, int s_col, int d_row, int d_col)
{
    if (d_col - s_col == -1) {
        return Directions::LEFT;
    } else if (d_col - s_col == 1) {
        return Directions::RIGHT;
    } else if (d_row - s_row == 1) {
        return Directions::DOWN;
    } else {
        return  Directions::UP;
    }
}

void game::Actor::hit(int damage)
{
    if (curr_hp_ - damage >= 0) {
        curr_hp_ -= damage;
    } else {
        curr_hp_ = 0;
    }
}

void game::Actor::heal(int restore)
{
    if (curr_hp_ + restore <= max_hp_) {
        curr_hp_ += restore;
    } else {
        curr_hp_ = max_hp_;
    }
}

void game::Actor::collide(game::ProjectileActor &other, const shared_ptr<game::Map> map)
{
    this->collide(*static_cast<ActiveActor*>(&other), map);
}

void game::Actor::collide(game::ActiveActor &other, const shared_ptr<game::Map> map)
{
    this->collide(*static_cast<Actor*>(&other), map);
}

void game::Actor::decrease_mana(int amount)
{
    if (curr_mana_ - amount >= 0) {
        curr_mana_ -= amount;
    } else {
        curr_mana_ = 0;
    }
}

void game::Actor::restore_mana(int amount)
{
    if (curr_mana_ + amount <= max_mana_) {
        curr_mana_ += amount;
    } else {
        curr_mana_ = max_mana_;
    }
}

void game::EnemyActor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), other.attack_damage());
}

short game::EnemyActor::color_pair() const
{
    double hp_percent = static_cast<double>(curr_hp_) / max_hp_;
    if (hp_percent >= 0.75) {
        return Colors::DEFAULT;
    } else if (0.25 <= hp_percent && hp_percent < 0.75) {
        return Colors::YELLOW_BLACK;
    } else {
        return Colors::RED_BLACK;
    }
}

void game::TargetActor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_target_reached();
}

void game::TeacherActor::move(game::GameControls controls, const shared_ptr<game::Map> map)
{
    MapSearchResult player_search = map->find_player_near(row(), col());

    if (player_search.is_found) {
        map->get_cell(player_search.row, player_search.col)->actor()->collide(*this, map);
        return;
    }

    //10% chance shooting
    if (RndHelper::rand_yes_no(0.10) == YesOrNo::YES) {
        shoot();
        return;
    }

    if (RndHelper::rand_yes_no(0.1) == YesOrNo::YES) {
        direction( RndHelper::rand_direction());
    }

    int desired_row = row();
    int desired_col = col();

    direction_to_coord(direction(), desired_row, desired_col);

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    if (!map->get_cell(desired_row, desired_col)->actor()->is_transparent()) {
        MapSearchResult search_result = map->find_transparent_near(row(), col());

        if (search_result.is_found) {
            direction(coord_to_direction(row(), col(), search_result.row, search_result. col));
            map->get_cell(search_result.row, search_result.col)->actor()->collide(*this, map);
        }
    }

    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

game::Directions game::RndHelper::rand_direction()
{
    int rnd_direction = rand() % 4;

    switch (rnd_direction) {
        case 0:
            return Directions::UP;
        case 1:
            return Directions::LEFT;
        case 2:
            return Directions::DOWN;
        case 3:
            return Directions::RIGHT;
        default:
            return Directions::UP;
    }
}

game::YesOrNo game::RndHelper::rand_yes_no(double yes_percent)
{
    int rnd_choice = rand();
    if (rnd_choice <= RAND_MAX * yes_percent) {
        return YesOrNo::YES;
    } else {
        return YesOrNo::NO;
    }
}

void game::HealPotionActor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    //"kill" potion so we can stand on its place
    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), max_hp());
    EventManager::instance().add_move(other.get_ptr(), row_, col_);
    EventManager::instance().add_heal(other.get_ptr(), heal_);
}

void game::ActiveActor::shoot()
{
    weapon()->shoot(row(), col(), direction());
}

game::ActiveActor::ActiveActor(int row, int col, game::Directions direction,  char icon, int hit_points,
                               int attack_damage, short color_pair, int max_mana)
        :  Actor(row, col, icon, hit_points, attack_damage, color_pair, max_mana), direction_(direction)
{
    weapon_ = std::make_shared<StoneWeapon>();
}

void game::ActiveActor::collide(game::ProjectileActor& other, const shared_ptr<game::Map> map)
{
    if (other.is_dead()) {
        return;
    }

    other.kill();

    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), other.attack_damage());
}

void game::ProjectileActor::move(game::GameControls controls, const shared_ptr<game::Map> map)
{
    int desired_row = row();
    int desired_col = col();

    direction_to_coord(direction(), desired_row, desired_col);

    if (!map->is_inbound(desired_row, desired_col)) {
        return;
    }

    map->get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

void game::Wall::collide(game::ProjectileActor& other, const shared_ptr<game::Map> map)
{
    other.kill();
}

void game::ProjectileActor::collide(game::ActiveActor& other, const shared_ptr<game::Map> map)
{
    if (!is_dead()) {
        other.collide(*this, map);
    }

    EventManager::instance().add_move(other.get_ptr(), row(), col());
}

void game::ProjectileActor::collide(game::ProjectileActor &other, const shared_ptr<game::Map> map)
{
    if (is_dead()) {
        EventManager::instance().add_move(other.get_ptr(), row(), col());
    } else {
        kill();
        other.kill();
    }
}

void game::ManaPotionActor::collide(game::MainCharActor& other, const shared_ptr<game::Map> map)
{
    EventManager::instance().add_damage(other.get_ptr(), get_ptr(), max_hp());
    EventManager::instance().add_move(other.get_ptr(), row_, col_);
    EventManager::instance().add_mana(other.get_ptr(), restore_);
}
