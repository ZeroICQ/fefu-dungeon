#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include "actors.h"

namespace game {

enum class GameStatus {in_progress, stop};

enum class GameControls {up, down, left, right};

class MapCell
{
public:
    MapCell(Actor& actor, FloorActor& floor) : actor_(actor), floor_(floor) {}
    ~MapCell();

    const Actor& actor() { return actor_; }
    const FloorActor& floorActor() { return floor_; }

private:
    Actor& actor_;
    FloorActor& floor_;

};

class Level
{
public:
    Level(const std::vector<std::string>& map_sketch, const std::vector<std::string>& floor_map_sketch);
    ~Level();

    const std::vector<std::vector<MapCell*>>& get_map() { return map_cells_;};
    const std::vector<const Actor*> get_playable();
private:
    std::vector<std::vector<MapCell*>> map_cells_;
};


class Game
{
public:
    Game();
    ~Game();
    GameStatus make_turn(GameControls control);
    const std::vector<std::vector<MapCell*>>& get_level() { return level_->get_map(); }
private:
     Level* level_;
};

}//namespace game
