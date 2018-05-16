#pragma once

#include <memory>
#include "map.h"

namespace game {

class MapLoader {
public:
    MapLoader() : level_dir("../levels/"),
                  level_filenames({"test_1","test_2", "test_big", "test_small"}) {};

    std::shared_ptr<Map> load_map(int level) const;

    int size() { return static_cast<int>(level_filenames.size()); }
private:
    std::string level_dir;
    std::vector<std::string> level_filenames;

};

}//namespace game