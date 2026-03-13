#include <algorithm>
#include <iostream>
#include <random>
#include <typeinfo>
#include <utility>
#include <vector>
#include <memory>

#ifndef MOVES
#define MOVES

struct Status {
    virtual ~Status() = default;
    bool self;
    int level;
    std::string name;
    Status() : level(0) {}
    Status(int _level, bool _self, std::string _name) : level(_level), self(_self), name(_name) {}

    virtual void decreaseLevel() { level--; }
    virtual int getDmg() { return 0; }
    virtual int getBlock() { return 0; }
    virtual double getScaler() { return 0; }
    virtual void updateDmg() {}
    virtual bool operator==(const Status st) { return typeid(this) == typeid(st); }
};

struct Burning : public Status {
    int dmg;
    Burning(int _level, bool _self) : Status(_level, _self, "Burning"), dmg(_level) {}

    static std::shared_ptr<Burning> create(int _level, bool _self) {
        return std::make_shared<Burning>(_level, _self);
    }

    int getDmg() override { return dmg; }
    void updateDmg() override { dmg = level;}
};

struct Bleed : public Status {
    int dmg;
    Bleed(int _level, bool _self) : Status(_level, _self, "Bleed"), dmg(_level) {}

    static std::shared_ptr<Bleed> create(int _level, bool _self) {
        return std::make_shared<Bleed>(_level, _self);
    }

    void decreaseLevel() override {
        level--;
        dmg--;
    }

    int getDmg() override { return dmg; }
    void updateDmg() override { dmg = level;}
};

struct Shock : public Status {
    Shock(int _level, bool _self) : Status(_level, _self, "Shock") {}
    static std::shared_ptr<Shock> create(int _level, bool _self) {
        return std::make_shared<Shock>(_level, _self);
    }
};

struct Defense : public Status {
    int block;
    Defense(int _block, bool _self) : Status(1, _self, "Defense"), block(_block) {}
    static std::shared_ptr<Defense> create(int _block, bool _self) {
        return std::make_shared<Defense>(_block, _self);
    }
    int getBlock() override { return block; }
};

struct Weakness : public Status {
    int block;
    Weakness(int _level, bool _self) : Status(_level, _self, "Weakness"), block(-_level) {}
    static std::shared_ptr<Weakness> create(int _level, bool _self) {
        return std::make_shared<Weakness>(_level, _self);
    }
    int getBlock() override { return block; }
};

struct Strength : public Status {
    double scaler;
    Strength(int _level, bool _self) : Status(_level, _self, "Strength"), scaler(1 + _level * 0.25) {}
    static std::shared_ptr<Strength> create(int _level, bool _self) {
        return std::make_shared<Strength>(_level, _self);
    }
    double getScaler() override { return scaler; }
};

struct Invisible : public Status {
    Invisible(int _level, bool _self) : Status(_level, _self, "Invisible") {}
    static std::shared_ptr<Invisible> create(int _level, bool _self) {
        return std::make_shared<Invisible>(_level, _self);
    }
};

class Move {
  public:
    std::string name;
    int attackPower;
    int heal;
    int cost;
    std::vector<std::shared_ptr<Status>> status;
    Move() {}
    Move(std::string _name, int _attackPower, int _heal, int _cost, std::vector<std::shared_ptr<Status>> _status)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status(_status) {}

    Move(std::string _name, int _attackPower, int _heal, int _cost, std::shared_ptr<Status> _status)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status{_status} {}

    Move(std::string _name, int _attackPower, int _heal, int _cost)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status(0) {}
};

#endif