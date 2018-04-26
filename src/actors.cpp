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
}

game::FloorActorFactory::FloorActorFactory()
{
    add_actor<EmptyFloor>();
}


void game::MainCharActor::move(game::GameControls controls, shared_ptr<game::Map> map) const
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



//void game::GuardActor::move(game::GameControls controls, game::Map& map) {
//    ActiveActor::move(controls, map);
//    enum Directions {up, right, down, left, count};
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    auto gen_rnd_direction = uniform_int_distribution<>(0, Directions::count-1);
//
//    int desired_row = row();
//    int desired_col = col();
//
//    switch (gen_rnd_direction(gen)) {
//        case Directions::up:
//            desired_row -= 1;
//            break;
//        case Directions::right:
//            desired_col += 1;
//            break;
//        case Directions::down:
//            desired_row += 1;
//            break;
//        case Directions::left:
//            desired_col -= 1;
//            break;
//        default:
//            break;
//    }
//
//    if (!map.is_inbound(desired_row, desired_col)) {
//        return;
//    }
//
//    map.get_cell(desired_row, desired_col)->actor()->collide(*this, map);
//}

shared_ptr<game::MapCell> game::Actor::get_shared_ptr(int row, int col, shared_ptr<game::Map> map)
{
    return map->get_cell(row, col);
}

void game::EmptyActor::collide(const game::ActiveActor &other, shared_ptr<game::Map> map)
{
    EventManager::instance().add_move(get_shared_ptr(other.row(), other.col(), map)->actor(), row_, col_);
}

void game::GuardActor::move(game::GameControls controls, const shared_ptr<game::Map> map) const {
    enum Directions {up, right, down, left, count};

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
