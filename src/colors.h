#pragma once

namespace game {


class Colors
{
public:
    static const short DEFAULT = 0;
    static const short FULL_WHITE = 1;
    static void initialize();
private:
    Colors() = delete;
    ~Colors() = delete;

};

}//namespace game

