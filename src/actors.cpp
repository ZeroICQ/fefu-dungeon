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
        return shared_ptr<BaseT>(nullptr);
    }
    return shared_ptr<BaseT>(constructors_[icon](row, col));
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
