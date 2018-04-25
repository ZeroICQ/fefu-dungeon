#include <vector>
#include <random>
#include "actors.h"

using std::vector;
using std::shared_ptr;
using std::uniform_int_distribution;

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

//void game::MainCharActor::move(game::GameControls controls, Map& map)
//{
////    if (!can_make_turn()) {
////        return;
////    }
//
//    ActiveActor::move(controls, map);
//
//    auto desired_row = row();
//    auto desired_col = col();
//
//    switch (controls) {
//        case GameControls::move_up:
//            desired_row -= 1;
//            break;
//        case GameControls::move_right:
//            desired_col += 1;
//            break;
//        case GameControls::move_down:
//            desired_row += 1;
//            break;
//        case GameControls::move_left:
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

//void game::EmptyActor::collide(game::ActiveActor& other, game::Map& map)
//{
//    map.move_actor(other.row(), other.col(), row(), col());
//
//}
//
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
