#include <vector>
#include "actors.h"

using std::vector;
using std::shared_ptr;


template<class BaseT>
template<class ActorT>
void game::BaseFactory<BaseT>::add_actor()
{
    constructors_[ActorT().map_icon()] = [](int row, int col){return new ActorT(row, col);};
}

template<class BaseT>
shared_ptr<BaseT> game::BaseFactory<BaseT>::create(char icon, int row, int col)
{
    if (!constructors_.count(icon)) {
        return std::shared_ptr<BaseT>(nullptr);
    }
    return std::shared_ptr<BaseT>(constructors_[icon](row, col));
}


game::ActorFactory::ActorFactory()
{
    add_actor<Wall>();
    add_actor<MainCharActor>();
    add_actor<EmptyActor>();
}

game::FloorActorFactory::FloorActorFactory()
{
    add_actor<EmptyFloor>();
}

//?????????????????????????????????????????????????????????????????????????????????
//ASK: так делают?
template class game::BaseFactory<game::Actor>;
template class game::BaseFactory<game::FloorActor>;

void game::MainCharActor::move(game::GameControls controls, Map& map)
{
    if (!can_make_turn()) {
        return;
    }

    ActiveActor::move(controls, map);

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

    if (!map.is_inbound(desired_row, desired_col)) {
        return;
    }

    map.get_cell(desired_row, desired_col)->actor()->collide(*this, map);
}

void game::EmptyActor::collide(game::MainCharActor& other, game::Map& map)
{
    map.move_actor(other.row(), other.col(), row(), col());

}

void game::ActiveActor::move(game::GameControls control, game::Map& map)
{
    can_make_turn(false);
}
