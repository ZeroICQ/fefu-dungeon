#pragma once

#include <memory>
#include <string>
#include <vector>
#include "actors.h"

namespace game {

class MapCell;
class MapConstIterator;
class MapIterator;

class Actor;
class FloorActor;
class EmptyActor;

struct MapSearchResult
{
    explicit MapSearchResult(bool is_found = false, int row = -1, int col = -1)
            : is_found(is_found), row(row), col(col) {}

    bool is_found;
    int row;
    int col;
};

class Map
{
public:
    Map(const std::vector<std::string>& map_sketch, const std::vector<std::string>& floor_map_sketch);
    std::unique_ptr<MapConstIterator> const_iterator() const;
    std::unique_ptr<MapIterator> iterator() const;
    bool is_inbound(int row, int col) const;
    int width() const { return static_cast<int>(map_cells_[0].size()); }
    int height() const { return static_cast<int>(map_cells_.size()); }
    std::shared_ptr<game::MapCell> get_cell(int row, int col) const { return map_cells_[row][col]; };
    void move_actor(int row_from, int col_from, int row_to, int col_to, std::shared_ptr<Actor> replace_actor = nullptr);
    void remove_actor(int row, int col, std::shared_ptr<Actor> replace_actor = nullptr);

    std::shared_ptr<game::Actor> get_main_char() { return main_char_; };
    MapSearchResult find_player_near(int row, int col) const;
    MapSearchResult find_transparent_near(int row, int col) const;
private:
    std::vector<std::vector<std::shared_ptr<game::MapCell>>> map_cells_;
    std::shared_ptr<game::Actor> main_char_;
};


class MapCell
{
public:
    //ASK: ok? no copy?
    MapCell(const std::shared_ptr<Actor>& actor,
            const std::shared_ptr<FloorActor>& floor)
        : actor_(actor), floor_(floor) {}

    std::shared_ptr<Actor> actor() const { return actor_; }
    void    actor(std::shared_ptr<Actor> replace) { actor_ = replace; }

    std::shared_ptr<FloorActor> floor() const { return floor_; }
    void floor(std::shared_ptr<FloorActor> replace) { floor_ = replace; }
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

    //ASK: как сделать константый геттер
    const std::shared_ptr<const Actor> actor() const { return MapIterator::actor(); };
    const std::shared_ptr<const FloorActor> floor() const { return MapIterator::floor(); }
};

} //namespace game
