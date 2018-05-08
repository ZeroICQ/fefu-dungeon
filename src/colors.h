#pragma once

namespace game {


class Colors
{
public:
    static const short DEFAULT = 0;
    static const short FULL_WHITE = 1;
    static const short YELLOW_BLACK = 2;
    static const short RED_BLACK = 3;
    static const short CYAN_BLACK = 4;
    static const short WHITE_RED = 5;

    static void initialize();
private:
    Colors() = delete;
    ~Colors() = delete;
};

}//namespace game

