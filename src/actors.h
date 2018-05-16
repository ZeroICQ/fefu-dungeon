#pragma once

#include <ncurses.h>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include <chrono>
#include "statuses.h"
#include "map.h"
#include "colors.h"
#include "weapons.h"
#include "cfg_loader.h"

namespace game {

enum class Directions {UP, RIGHT, DOWN, LEFT, STAY};
enum class YesOrNo {YES, NO};

class RndHelper
{
public:
    static Directions rand_direction();
    static YesOrNo rand_yes_no(double yes_percent);
private:
    RndHelper() = default;
};


//fd circular dependency
class Map;

class MapCell;
class MainCharActor;
class EmptyActor;
class EnemyActor;
class ActiveActor;
class Weapon;
class ProjectileActor;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
    explicit Actor(int row = 0, int col = 0, double speed = 1.0, char icon = '-', int max_hp = 100,
                   int attack_damage = 0, short color_pair = Colors::DEFAULT,
                   int max_mana = 0)
            : row_(row), col_(col), move_speed_{speed}, map_icon_(icon), max_hp_(max_hp), curr_hp_(max_hp),
              attack_damage_{attack_damage}, color_pair_(color_pair), max_mana_(max_mana), curr_mana_(max_mana) {}
    //ASK: ???
    virtual ~Actor() = default;

    std::shared_ptr<Actor> get_ptr();

    char map_icon() const { return map_icon_; }

    int row() const { return row_; }
    void row(int replace) { row_ = replace; }
    int col() const { return col_; }
    void col(int replace) { col_  = replace; }

    void set_pos(int r_row, int r_col) { row(r_row); col(r_col); }

    virtual void move(GameControls controls, const std::shared_ptr<Map> map,
                      const std::chrono::time_point<std::chrono::steady_clock>& curr_tp) {}

    virtual void collide(Actor& other, const std::shared_ptr<Map> map) {};
    virtual void collide(ActiveActor& other, const std::shared_ptr<Map> map);
    virtual void collide(MainCharActor& other, const std::shared_ptr<Map> map);
    virtual void collide(EnemyActor& other, const std::shared_ptr<Map> map);
    virtual void collide(ProjectileActor& other, const std::shared_ptr<Map> map);

    //flags
    virtual bool is_transparent() const { return false; }
    virtual bool is_playable() const { return false; }
    virtual bool is_dead() const { return curr_hp_<= 0; }

    //getters and setters
    int max_hp() const { return max_hp_; }
    int curr_hp() const { return curr_hp_; }

    int max_mana() const { return max_mana_; }
    int curr_mana() const { return curr_mana_; }

    void decrease_mana(int amount);
    void restore_mana(int amount);

    void hit(int damage);
    void kill() { curr_hp_ = 0; }
    void heal(int restore);

    int attack_damage() { return attack_damage_; }

    virtual short color_pair() const { return color_pair_; }
    //TODO: find a better place
    static void direction_to_coord(Directions direction, int& row, int& col);
    static Directions coord_to_direction(int s_row, int s_col, int d_row, int d_col);

    bool is_made_turn() const { return is_made_turn_; }
    void is_made_turn(bool val)  { is_made_turn_ = val; }

protected:
    int row_;
    int col_;
    //cells per second
    double move_speed_;
    std::chrono::milliseconds attack_speed_{1000};//msek
    std::chrono::time_point<std::chrono::steady_clock> last_updated_ = std::chrono::steady_clock::now();

    char map_icon_;
    int max_hp_;
    int curr_hp_;
    int attack_damage_;
    short color_pair_;
    bool is_made_turn_ = false;
    int max_mana_;
    int curr_mana_;

    double get_path(const std::chrono::milliseconds& delta_t) const;

};


class FloorActor : public Actor
{
public:
    explicit FloorActor(int row = 0, int col = 0, char icon = 'F') : Actor(row, col, 0, icon) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class EmptyFloor : public FloorActor
{
public:
    explicit EmptyFloor(int row = 0, int col = 0, char icon = ' ') : FloorActor(row, col, icon) {}
    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
};


class EmptyActor : public Actor
{
public:
    explicit EmptyActor(int row = 0, int col = 0, char icon = ' ') : Actor(row, col, 0, icon) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ActiveActor& other, const std::shared_ptr<Map> map) override;

    bool is_transparent() const final { return true; }
};


class Wall : public Actor
{
public:
    explicit Wall(int row = 0, int col = 0, char icon = '#', int max_hp = 100,
            int attack_damage = 0, short colors_pair = Colors::FULL_WHITE)
            : Actor(row, col, 0, icon, max_hp, attack_damage, colors_pair) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ActiveActor& other, const std::shared_ptr<Map> map) override { }
    void collide(ProjectileActor& other, const std::shared_ptr<Map> map) override;

};


class TargetActor : public Wall
{
public:
    explicit TargetActor(int row = 0, int col = 0, char icon = 'Z', int max_hp = 100,
            int attack_damage = 0, short colors_pair = Colors::WHITE_CYAN)
            : Wall(row, col, icon, max_hp, attack_damage, colors_pair) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(MainCharActor& other, const std::shared_ptr<Map> map) override;

};


class ActiveActor : public Actor
{
public:
    explicit ActiveActor(int row = 0, int col = 0, double speed = 1.0, Directions direction = Directions::UP,
                         char icon = 'A', int hit_points = 0,
                         int attack_damage = 0, short color_pair = Colors::DEFAULT, int max_mana = 500);

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ProjectileActor& other, const std::shared_ptr<Map> map) override;

    Directions direction() const { return direction_; }
    void direction(Directions r_dir) { direction_ = r_dir; }

    virtual std::shared_ptr<Weapon> weapon() const { return weapon_; }

    virtual void shoot();

protected:
    Directions direction_;
    std::shared_ptr<Weapon> weapon_;
};


class MainCharActor : public ActiveActor
{
public:
    explicit MainCharActor(int row = 0, int col = 0, game::Directions direction = Directions::STAY,
                           char icon = Cfg::getMainChar().icon,
                           int hit_points = Cfg::getMainChar().max_hp,
                           int attack_damage = Cfg::getMainChar().attack_damage,
                           short color_pair = Cfg::getMainChar().color_pair,
                           int max_mana = Cfg::getMainChar().max_mana);

    void move(GameControls controls, std::shared_ptr<Map> map,
              const std::chrono::time_point<std::chrono::steady_clock>& curr_tp) override;

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(EnemyActor& other, const std::shared_ptr<Map> map) override;

    void shoot() override;

    //flags
    bool is_playable() const override { return true; }

    std::shared_ptr<Weapon> weapon() const override;

protected:
    void next_weapon();
    std::vector<std::shared_ptr<Weapon>> weapons_;
    int weapon_number_ = 0;
};


class EnemyActor : public ActiveActor
{
public:
    explicit EnemyActor(int row = 0, int col = 0, double speed = 1.0,  Directions direction = Directions::UP, char icon = 'E',
                        int hit_points = 0, int attack_damage = 100, short color_pair = Colors::DEFAULT)
            : ActiveActor(row, col, speed, direction, icon, hit_points, attack_damage, color_pair) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(EnemyActor& other, const std::shared_ptr<Map> map) override {};
    void collide(MainCharActor& other, const std::shared_ptr<Map> map) override;

    short color_pair() const override;
};


class GuardActor : public EnemyActor
{
public:
    explicit GuardActor(int row = 0, int col = 0, double speed = 2.0, Directions direction = RndHelper::rand_direction(),
                       char icon = 'G', int hit_points = 100, int attack_damage = 20)
            : EnemyActor(row, col, speed, direction, icon, hit_points, attack_damage) {}

    void move(GameControls controls, const std::shared_ptr<Map> map,
              const std::chrono::time_point<std::chrono::steady_clock>& curr_tp) override;
    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
//    void move(GameControls control, Map& map) override;

//    void collide(Actor& other, Map& map) override { return other.collide(*this, map); }
};

class TeacherActor : public EnemyActor
{
public:
    explicit TeacherActor(int row = 0, int col = 0, double speed = 3.0, Directions direction = RndHelper::rand_direction(),
            char icon = 'T', int hit_points = 1000, int attack_damage = 25, short color_pair = Colors::DEFAULT)
             : EnemyActor(row, col, speed, direction, icon, hit_points, attack_damage, color_pair) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void move(GameControls controls, const std::shared_ptr<Map> map,
              const std::chrono::time_point<std::chrono::steady_clock>& curr_tp) override;
};

//TODO: should be floor actor
class PickupActor : public Wall
{
public:
    explicit PickupActor(int row = 0, int col = 0, char icon = 'P', int max_hp = 1,
                         int attack_damage = 0, short color_pair = Colors::DEFAULT)
            : Wall(row, col, icon, max_hp, attack_damage, color_pair) {};

    void collide(Actor &other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    //should really replace potions to the floor
//    bool is_transparent() const override { return true; }
};

class HealPotionActor : public PickupActor
{
public:
    explicit HealPotionActor(int row = 0, int col = 0, char icon = 'H', int max_hp = 1,
                             int attack_damage = 0, short color_pair = Colors::WHITE_RED, int heal = 100)
            : PickupActor{row, col , icon, max_hp, attack_damage, color_pair}, heal_{heal} {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(MainCharActor& other, const std::shared_ptr<Map> map) override;

protected:
    int heal_;
};

class ManaPotionActor : public PickupActor
{
public:
    ManaPotionActor(int row = 0, int col = 0, char icon = 'M', int max_hp = 1,
                    int attack_damage = 0, short color_pair = Colors::WHITE_BLUE, int restore = 500)
            : PickupActor{row, col , icon, max_hp, attack_damage, color_pair}, restore_{restore} {}

    void collide(Actor &other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(MainCharActor& other, const std::shared_ptr<Map> map) override;
protected:
    int restore_;
};


class ProjectileActor : public ActiveActor
{
public:
    explicit ProjectileActor(int row = 0, int col = 0, Directions direction = Directions::UP, char icon = 'p',
                             int hit_points = 1, int attack_damage = 100, short color_pair = Colors::DEFAULT,
                             double speed = 1.0)
            : ActiveActor(row, col, speed, direction, icon, hit_points, attack_damage, color_pair) {}

    void collide(Actor& other, const std::shared_ptr<Map> map) override { other.collide(*this, map); }
    void collide(ActiveActor& other, const std::shared_ptr<Map> map) override;
    void collide(ProjectileActor& other, const std::shared_ptr<Map> map) override;
    void move(GameControls controls, const std::shared_ptr<Map> map,
              const std::chrono::time_point<std::chrono::steady_clock>& curr_tp) override;

    bool is_transparent() const override { return is_dead(); }
};

class FireballActor : public ProjectileActor
{
public:
    explicit FireballActor(int row = 0, int col = 0, Directions direction = Directions::UP, char icon = '*',
                           int hit_points = 1, int attack_damage = 100, short color_pair = Colors::RED_BLACK)
            : ProjectileActor(row, col, direction, icon, hit_points, attack_damage, color_pair, 10.0) {}
};

class StoneActor : public ProjectileActor
{
public:
    explicit StoneActor(int row = 0, int col = 0, Directions direction = Directions::UP, char icon = 'o', int hit_points = 1,
                           int attack_damage = 500, short color_pair = Colors::CYAN_BLACK)
            : ProjectileActor(row, col, direction, icon, hit_points, attack_damage, color_pair, 6.0) {}
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