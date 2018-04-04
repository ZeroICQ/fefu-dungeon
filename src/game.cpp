#include <vector>
#include "game.h"
#include "actors.h"

using std::vector;

game::Level::Level(const std::vector<std::string>& map_sketch, const std::vector<std::string>& floor_map_sketch)
{
    ActorFactory actor_factory;
    FloorActorFactory floor_factory;
    auto width = static_cast<int>(map_sketch[0].size());

    map_cells_.reserve(map_sketch.size() / width + 1);

    for (int y = 0; y < (int) map_sketch.size(); y++) {
        map_cells_.emplace_back();
        map_cells_[y].reserve(static_cast<unsigned long>(width));
        for (int x = 0; x < width; x++) {
            map_cells_[y].push_back(new MapCell(
                    *actor_factory.create(map_sketch[y][x], y, x),
                    *floor_factory.create(floor_map_sketch[y][x], y, x)));
        }
    }
}

game::Level::~Level()
{
    for (auto &line : map_cells_) {
        for (auto &cell : line) {
            delete cell;
        }
    }
    map_cells_.clear();
}

game::MapCell::~MapCell()
{
    delete &actor_;
    delete &floor_;
}

game::Game::Game()
{
    const std::vector<std::string> map = {
            "###################################################################",
            "#                                                                 #",
            "#           ###########                                           #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                             #####################               #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "#                                                                 #",
            "###################################################################"};

    const std::vector<std::string> floor_map = {
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   ",
            "                                                                   "};

    level_ = new Level(map, floor_map);
}

game::Game::~Game()
{
    delete level_;
}

game::GameStatus game::Game::make_turn(game::GameControls control)
{
    return GameStatus::in_progress;
}
