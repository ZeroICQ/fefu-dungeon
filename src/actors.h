#pragma once

#include <string>
#include <functional>
#include <map>
#include <memory>
#include "statuses.h"
#include "map.h"
#include "event_system.h"

namespace game {

//fd circular dependency
class Map;
class MapCell;
class MainCharActor;
class EmptyActor;
class ActiveActor;

class Actor
{
public:
    explicit Actor(int row = 0, int col = 0, char icon = '-') : row_(row), col_(col), map_icon_(icon) {}

    //MyLittleHack
    std::shared_ptr<MapCell> get_shared_ptr(int row, int col, std::shared_ptr<Map> map);

    char map_icon() const { return map_icon_; }
    int row() const { return row_; }
    void row(int replace) { row_ = replace; }
    int col() const { return col_; }
    void col(int replace) { col_  = replace; }

    void set_pos(int r_row, int r_col) { row(r_row); col(r_col); }

    virtual void move(GameControls controls, const std::shared_ptr<Map> map) const {}

    virtual void collide(const Actor& other, const std::shared_ptr<Map> map) const = 0;
//    virtual void collide(const MainCharActor& other, const std::shared_ptr<Map> map) {}
    virtual void collide(const ActiveActor& other, const std::shared_ptr<Map> map) {};

    //flags
    virtual bool is_transparent() { return false; }

protected:
    int row_;
    int col_;
    char map_icon_;
};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int row = 0, int col = 0, char icon = 'F') : Actor(row, col, icon) {}

    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }
};


class EmptyFloor : public FloorActor
{
public:
    explicit EmptyFloor(int row = 0, int col = 0, char icon = ' ') : FloorActor(row, col, icon) {}
    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }
};


class EmptyActor : public Actor
{
public:
    explicit EmptyActor(int row = 0, int col = 0, char icon = ' ') : Actor(row, col, icon) {}

    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }
    void collide(const ActiveActor& other, const std::shared_ptr<Map> map) override;

    bool is_transparent() final { return true; }
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#') : Actor(row, col, icon) {}

    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }

};


class ActiveActor : public Actor
{
public:
    explicit ActiveActor(int row = 0, int col = 0, char icon = 'A') :  Actor(row, col, icon) {}
    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }

};


class MainCharActor : public ActiveActor
{
public:
    explicit MainCharActor(int row = 0, int col = 0, char icon ='S') : ActiveActor(row, col, icon) {}

    void move(GameControls controls, std::shared_ptr<Map> map) const override;

    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }
};


class EnemyActor : public ActiveActor
{
public:
    explicit EnemyActor(int row = 0, int col = 0, char icon = 'E') : ActiveActor(row, col, icon) {}

    void collide(const Actor &other, const std::shared_ptr<Map> map) const override { other.collide(*this, map); }
};


class GuardActor : public EnemyActor
{
public:
    explicit GuardActor(int row = 0, int col = 0, char icon ='G') : EnemyActor(row, col, icon) {}

    void move(GameControls controls, const std::shared_ptr<Map> map) const override;
//    void move(GameControls control, Map& map) override;

//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};


template <class BaseT>
class BaseFactory
{
public:
    template <class ActorT>
    void add_actor()
    {
        constructors_[ActorT().map_icon()] = [](int row, int col){return new ActorT(row, col);};
    }

    //CAN return nullptr
    std::shared_ptr<BaseT> create(char icon, int row, int col)
    {
        if (!constructors_.count(icon)) {
            return std::shared_ptr<BaseT>(nullptr);
        }
        return std::shared_ptr<BaseT>(constructors_[icon](row, col));
    }

private:
    std::map<char, std::function<BaseT*(int, int)>> constructors_;
};


class ActorFactory : public BaseFactory<Actor>
{
public:
    explicit ActorFactory();
};


class FloorActorFactory : public BaseFactory<FloorActor>
{
public:
    explicit FloorActorFactory();
};

} //namespace game