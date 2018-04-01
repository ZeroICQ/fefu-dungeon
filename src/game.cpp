#include <vector>
#include "game.h"
#include "actors.h"

using std::vector;

game::Level::Level(const vector<char> &sketch, int width)
{
    ActorFactory factory;

    cells.reserve(sketch.size() / width + 1);

    for (int i = 0; i < (int) sketch.size(); i++) {
        auto y = i / width;
        auto x = i - y * width;

        if (x == 0) {
            cells.emplace_back();
            cells[y].reserve(static_cast<unsigned long>(width));
        }
        cells[y].push_back(new MapCell(*factory.create(sketch[i], y, x), *new Floor(y,x)));
    }
}

game::Level::~Level()
{
    for (auto &line : cells) {
        for (auto &cell : line) {
            delete cell;
        }
    }
    cells.clear();
}

game::MapCell::~MapCell()
{
    delete &object_;
    delete &floor_;
}

game::Game::Game()
{
    const std::vector<char> sample_map = {
            '#', '#', '#', '#', '#',
            '#', ' ', ' ', ' ', '#',
            '#', ' ', ' ', ' ', '#',
            '#', ' ', ' ', ' ', '#',
            '#', ' ', ' ', ' ', '#',
            '#', '#', '#', '#', '#'};

    level_ = new Level(sample_map, 5);
}

game::Game::~Game()
{
    delete level_;
}
