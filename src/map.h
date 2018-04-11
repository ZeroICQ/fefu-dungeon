#pragma once

#include <memory>
#include <string>
#include <vector>
#include "actors.h"

namespace game {

//ASK: is it ok to make such forward declarations?
class MapCell;
class MapConstIterator;
class MapIterator;


class Map
{
public:
    Map(const std::vector<std::string>& map_sketch, const std::vector<std::string>& floor_map_sketch);
    std::unique_ptr<MapConstIterator> const_iterator() const;
    std::unique_ptr<MapIterator> iterator() const;
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

    //ASK: нормально ли делать два геттера?
    std::shared_ptr<Actor> actor() const {return actor_; }

    std::shared_ptr<FloorActor> floor() const { return floor_; }
private:
    std::shared_ptr<Actor> actor_;
    std::shared_ptr<FloorActor> floor_;
};


class MapIterator
{
public:
    explicit MapIterator(const std::vector<std::vector<std::shared_ptr<game::MapCell>>>& map) : cells_(map) {}
    void next();
    bool is_end() const;

    const std::shared_ptr<game::Actor> actor() const;
    const std::shared_ptr<game::FloorActor> floor() const;
protected:
    const std::vector<std::vector<std::shared_ptr<game::MapCell>>>& cells_;
    int col_ = 0;
    int row_ = 0;
};

class MapConstIterator : public MapIterator
{
public:
    explicit MapConstIterator(const std::vector<std::vector<std::shared_ptr<game::MapCell>>>& map)
            : MapIterator(map) {}

    const std::shared_ptr<const Actor> actor() const { return MapIterator::actor(); };

    const std::shared_ptr<const FloorActor> floor() const { return MapIterator::floor(); }
};

} //namespace game
