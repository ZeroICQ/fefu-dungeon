#pragma once

#include <memory>
#include "map.h"

namespace game {

class MapLoader {
public:
    MapLoader() : level_dir("../levels/"),
                  level_filename({"test_1", "test_2"})
    {};

    std::shared_ptr<Map> load_map(int level) const;
private:
    std::string level_dir;
    std::vector<std::string> level_filename;

};

}//namespace game