#pragma once

#include <memory>
#include <string>
#include <vector>
#include "actors.h"

namespace game {

//ASK: is it ok to make such forward declarations?
class MapCell;
class MapConstIterator;


class Map
{
public:
    Map(const std::vector<std::string>& map_sketch, const std::vector<std::string>& floor_map_sketch);
    std::unique_ptr<MapConstIterator> iterator() const;
//    const std::unique_ptr<const game::MapCell>& get_cell(int col, int row) const { return &map_cells_[col][row]; };

private:
    std::vector<std::vector<std::shared_ptr<game::MapCell>>> map_cells_;
};


class MapCell
{
public:
    //ASK: ok? no copy?
    MapCell(const std::shared_ptr<Actor>& actor,
            const std::shared_ptr<FloorActor>& floor)
        : actor_(actor), floor_(floor) {}
    std::shared_ptr<Actor> actor() const {return actor_; }
    std::shared_ptr<FloorActor> floor() const { return floor_; }
private:
    std::shared_ptr<Actor> actor_;
    std::shared_ptr<FloorActor> floor_;
};


class MapConstIterator
{
public:
    explicit MapConstIterator(const std::vector<std::vector<std::shared_ptr<game::MapCell>>>& map) : cells_(map) {}
    void next();
    const std::shared_ptr<const game::MapCell> get_item() const;
    bool is_end() const;
private:
    //TODO: make check not empty
    int col_ = 0;
    int row_ = 0;
    const std::vector<std::vector<std::shared_ptr<game::MapCell>>>& cells_;
};

} //namespace game
