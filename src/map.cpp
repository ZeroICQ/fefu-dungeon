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

    actor_factory.create();
    auto map_sketch_size = static_cast<int>(map_sketch.size());

    auto cols = static_cast<int>(map_sketch.front().size());
    auto rows = static_cast<int>(map_sketch.size()) / cols
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
    new MapConstIterator(map_cells_);
    return std::unique_ptr<MapConstIterator>();
}

const shared_ptr<const game::MapCell> game::MapConstIterator::begin()
{
    col_++;
    if (col_ >= static_cast<int>(cells_[row_].size())) {
        col_ = 0;
        row_++;
    }
    return cells_[row_][col_];
}

bool game::MapConstIterator::is_end() const
{
    return static_cast<int>(cells_.size()) == row_ && static_cast<int>(cells_.back().size()) == col_;
}
