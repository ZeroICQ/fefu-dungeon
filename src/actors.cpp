#include <vector>
#include "actors.h"

using std::vector;

game::ActorFactory::ActorFactory()
{
    //leak?
    vector<Actor> object_actors = {
            Wall(),
    };

    for (auto const& actor : object_actors) {
        add_actor(actor, actor.map_icon());
//        delete &actor;//ASK: WHY SIGSEGV????????//
    }
}

template<class T>
bool game::ActorFactory::add_actor(T actor, char icon)
{
    constructors_[icon] = [](int y, int x){return new T(y, x);};
    return true;
}

game::Actor* game::ActorFactory::create(char icon, int y, int x)
{
    if (!constructors_.count(icon)) {
        return new Actor(y, x);//empty
    }

    return constructors_[icon](y, x);
}

