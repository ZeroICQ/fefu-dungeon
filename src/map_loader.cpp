#include <libconfig.h++>
#include <sstream>
#include "map_loader.h"

std::shared_ptr<game::Map> game::MapLoader::load_map(int level) const
{
    libconfig::Config level_cfg;
    level_cfg.readFile((level_dir + level_filename[level]).c_str());

    std::istringstream map_string(level_cfg.lookup("map"));
    std::istringstream floor_string (level_cfg.lookup("floor_map"));

    std::string line;

    std::vector<std::string> map_sketch;
    std::vector<std::string> floor_sketch;

    while (std::getline(map_string, line)) {
        map_sketch.push_back(line);
    }

    while (std::getline(floor_string, line)) {
        floor_sketch.push_back(line);
    }
    return std::make_shared<Map>(map_sketch, floor_sketch);
}
