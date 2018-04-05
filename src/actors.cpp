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
// так делают?
template class game::BaseFactory<game::Actor>;
template class game::BaseFactory<game::FloorActor>;

void game::MainCharActor::move(game::GameControls control)
{
    switch (control) {
        case GameControls::move_up:
            move_up();
            break;
        case GameControls::move_right:
            move_right();
            break;
        case GameControls::move_down:
            move_down();
            break;
        case GameControls::move_left:
            move_left();
            break;
        default:
            break;
    }
}

void game::ActiveActor::move_up()
{
    row_ -= 1;
}

void game::ActiveActor::move_right()
{
    col_ += 1;
}

void game::ActiveActor::move_down()
{
    row_ += 1;
}

void game::ActiveActor::move_left()
{
    col_ -= 1;
}
