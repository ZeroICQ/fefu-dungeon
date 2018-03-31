#pragma once

namespace cui {

class Ui
{
public:
    Ui() = default;
    ~Ui();

    void show_menu();

private:
    void initialize() const;
};

}//namespace cui
