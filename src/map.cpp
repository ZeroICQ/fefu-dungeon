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
                        actor_factory.create(map_sketch[row].c_str()[col], row, col),
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

bool game::Map::is_inbound(int row, int col) const
{
    return 0 <= col && col < width() && 0 <= row && row < height();
}

void game::Map::move_actor(int row_from, int col_from, int row_to, int col_to, shared_ptr<game::Actor> replace_actor)
{
    if (!replace_actor) {
        replace_actor = std::make_shared<EmptyActor>(row_from, col_from);
    }

    map_cells_[row_to][col_to]->actor(map_cells_[row_from][col_from]->actor());
    map_cells_[row_to][col_to]->actor()->set_pos(row_to, col_to);

    map_cells_[row_from][col_from]->actor(replace_actor);
    map_cells_[row_from][col_from]->actor()->set_pos(row_from, col_from);
}

void game::Map::remove_actor(int row, int col, shared_ptr<game::Actor> replace_actor)
{
    if (!replace_actor) {
        replace_actor = std::make_shared<EmptyActor>(row, col);
    }

    map_cells_[row][col]->actor(replace_actor);
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

