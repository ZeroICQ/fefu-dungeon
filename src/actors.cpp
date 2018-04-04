#include <vector>
#include "actors.h"

using std::vector;


template<class T>
template<class G>
bool game::Factory<T>::add_actor(G actor, char icon)
{
    constructors_[icon] = [](int y, int x){return new G(y, x);};
    return true;
}

template<class T>
T *game::Factory<T>::create(char icon, int y, int x)
{
    if (!constructors_.count(icon)) {
        return nullptr;
    }
    return constructors_[icon](y, x);
}

game::ActorFactory::ActorFactory()
{
    real_factory_.add_actor(Wall(), Wall().map_icon());
    real_factory_.add_actor(MainCharActor(), MainCharActor().map_icon());
    real_factory_.add_actor(VoidActor(), VoidActor().map_icon());
}

game::FloorActorFactory::FloorActorFactory()
{
    //ASK: optimize?
    real_factory_.add_actor(EmptyFloor(), EmptyFloor().map_icon());
}

game::Actor* game::ActorFactory::create(char icon, int y, int x)
{
    auto actor = real_factory_.create(icon, y, x);
    if (actor == nullptr) {
        return new VoidActor(y, x);
    }
    return actor;
}

game::FloorActor *game::FloorActorFactory::create(char icon, int y, int x)
{
    auto actor = real_factory_.create(icon, y, x);
    if (actor == nullptr) {
        return new FloorActor(y, x);
    }
    return actor;
}

void game::FloorActor::move(game::GameControls control)
{

}
