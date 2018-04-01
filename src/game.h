#include <ncurses.h>
#include <vector>
#include "actors.h"

namespace game {

enum class GameStatus {in_progress, exit};

class MapCell
{
public:
    MapCell(Actor& object, Actor& floor) : object_(object), floor_(floor) {}
    ~MapCell();
    void floor(Actor& floor_actor) { floor_ = floor_actor; }

private:
    Actor& object_;
    Actor& floor_;

};

class Level
{
public:
    Level(const std::vector<char>& sketch, int width);
    ~Level();
private:
    std::vector<std::vector<MapCell*>> cells;
};


class Game
{
public:
    Game();
    ~Game();
//    GameStatus start();
private:
     Level* level_;
};

}//namespace game
