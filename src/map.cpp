#include <memory>
#include "map.h"
#include "actors.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;

game::Map::Map(const vector<string>& map_sketch, const vector<string>& floor_map_sketch)
{
    ActorFactory actor_factory;
    FloorActorFactory floor_factory;
//    auto map_sketch_size = static_cast<int>(map_sketch.size());

    auto cols = static_cast<int>(map_sketch.front().size());
    auto rows = static_cast<int>(map_sketch.size());

    map_cells_.reserve(static_cast<ulong>(rows));

    for (int row = 0; row < rows; row++) {
        map_cells_.emplace_back();
        map_cells_.back().reserve(static_cast<unsigned long>(cols));
        for (int col = 0; col < cols; col++) {
            map_cells_.back().emplace_back(new MapCell(
                        actor_factory.create(map_sketch[row][col], row, col),
                        floor_factory.create(floor_map_sketch[row][col], row, col)
            ));
        }
    }

}

unique_ptr<game::MapConstIterator> game::Map::const_iterator() const
{
    return std::make_unique<MapConstIterator>(map_cells_);
}

unique_ptr<game::MapIterator> game::Map::iterator() const
{
    return std::make_unique<MapIterator>(map_cells_);
}

void game::MapIterator::next()
{
    col_++;
    if (col_ >= static_cast<int>(cells_[row_].size())) {
        col_ = 0;
        row_++;
    }
}

bool game::MapIterator::is_end() const
{
    return static_cast<int>(cells_.size()) <= row_;
}

const std::shared_ptr<game::Actor> game::MapIterator::actor() const
{
    return cells_[row_][col_]->actor();
}

const std::shared_ptr<game::FloorActor> game::MapIterator::floor() const
{
    return cells_[row_][col_]->floor();
}

