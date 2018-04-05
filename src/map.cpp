#include "map.h"
#include "actors.h"

using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;

game::Map::Map(const vector<string>& map_sketch, const vector<string>& floor_map_sketch)
{
    ActorFactory actor_factory;
    FloorActorFactory floor_factory;
    auto map_sketch_size = static_cast<int>(map_sketch.size());

    auto cols = static_cast<int>(map_sketch.front().size());
    auto rows = static_cast<int>(map_sketch.size());
                + (map_sketch_size % cols > 0 ? 1 : 0);

    map_cells_.reserve(static_cast<ulong>(rows));

    for (int row = 0; row < rows; row++) {
        map_cells_.emplace_back();
        map_cells_.back().reserve(static_cast<unsigned long>(cols));
        for (int col = 0; col < cols; col++) {
            map_cells_.back().emplace_back(new MapCell(
                        actor_factory.create(map_sketch[row][col], row, col),
                        floor_factory.create(map_sketch[row][col], row, col)
                    ));
        }
    }

}

unique_ptr<game::MapConstIterator> game::Map::iterator() const
{
    return std::unique_ptr<MapConstIterator>(new MapConstIterator(map_cells_));
}


bool game::MapConstIterator::is_end() const
{
    //TODO: do a better iterator
    return static_cast<int>(cells_.size()) <= row_;
}

const std::shared_ptr<const game::MapCell> game::MapConstIterator::get_item() const
{
    return cells_[row_][col_];
}

void game::MapConstIterator::next()
{
    col_++;
    if (col_ >= static_cast<int>(cells_[row_].size())) {
        col_ = 0;
        row_++;
    }
}
