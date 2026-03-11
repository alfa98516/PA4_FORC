#include <algorithm>
#include <iostream>
#include <random>
#include <typeinfo>
#include <utility>
#include <vector>

#ifndef MOVES
#define MOVES

struct Status {
    virtual ~Status() = default;
    int level;
    Status() : level(0) {}
    Status(int _level) : level(_level) {}

    virtual void decreaseLevel() { level--; }
    virtual int getDmg() { return 0; }
    virtual int getBlock() { return 0; }
    virtual double getScaler() { return 0; }

    virtual bool operator==(const Status st) { return typeid(this) == typeid(st); }
};

struct Burning : public Status {
    int dmg;
    Burning(int _level) : Status(_level), dmg(_level) {}
    int getDmg() override { return dmg; }
};

struct Bleed : public Status {
    int dmg;
    Bleed(int _level) : Status(_level), dmg(_level) {}

    void decreaseLevel() override {
        level--;
        dmg--;
    }

    int getDmg() override { return dmg; }
};

struct Shock : public Status {
    Shock(int _level) : Status(_level) {}
};

struct Defense : public Status {
    int block;
    Defense(int _block) : Status(1), block(_block) {}
    int getBlock() override { return block; }
};

struct Weakness : public Status {
    int block;
    Weakness(int _level) : Status(_level), block(-_level) {}

    int getBlock() override { return block; }
};

struct Strength : public Status {
    double scaler;

    Strength(int _level) : Status(_level), scaler(1 + _level * 0.25) {}

    double getScaler() override { return scaler; }
};

class Move {
  public:
    std::string name;
    int attackPower;
    int heal;
    int cost;
    std::vector<Status> status;
    Move() {}
    Move(std::string _name, int _attackPower, int _heal, int _cost, std::vector<Status> _status)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status(_status) {}

    Move(std::string _name, int _attackPower, int _heal, int _cost, Status _status)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status{_status} {}

    Move(std::string _name, int _attackPower, int _heal, int _cost)
        : name(_name), attackPower(_attackPower), heal(_heal), cost(_cost), status(0) {}
};

#endif
#ifndef ENEMY
#define ENEMY
class Enemy {
  public:
    std::random_device seed = std::random_device{
        "rdrand"}; // Looks for a device connected to the computer that can generate
                   // non-deterministic, true random numbers. Pretty intensive
                   // (around 300 clock cycles). We use this as the seed.
    virtual ~Enemy() = default;
    std::vector<Move> moves;
    int health;
    const int maxHealth;
    std::uniform_int_distribution<int> dist;
    int stamina;
    const int maxStamina;
    std::string name;
    std::string art;
    std::vector<Status> status;
    int defense;
    bool shocked;
    std::pair<Enemy*, Enemy*> allies;

  protected:
    virtual int baseAction() {
        statusManager();
        if (shocked) {
            return -1;
        }

        int moveIdx = dist(seed);

        return moveIdx;
    }

  protected:
    void statusManager() {
        for (Status st : status) {
            if (typeid(st) == typeid(Burning) || typeid(st) == typeid(Bleed)) {
                health -= st.getDmg();
            } else if (typeid(st) == typeid(Defense) || typeid(st) == typeid(Weakness)) {
                defense += st.getBlock();
            }

            st.decreaseLevel();

            if (st.level <= 0) {
                status.erase(std::remove(status.begin(), status.end(), st), status.end());
            }
        }
    }

    Enemy(std::string _name, int _health, int _stamina, int b, int nMoves)
        : health(_health), stamina(_stamina), name(_name), dist(0, b), moves(nMoves + 1),
          defense(0), maxStamina(_stamina), maxHealth(_health), shocked(false) {
        moves[0] = Move("Rest", 0, 0, 0);
        allies = std::make_pair(nullptr, nullptr);
    }
};

class Minion : public Enemy {
  public:
    explicit Minion() : Enemy("Minion", 50, 10, 3, 4) {
        moves[1] = Move("Kick", 10, 0, 2);
        moves[2] = Move("Observe", 0, 0, 4, Weakness(2));
        moves[3] = Move("Block", 0, 0, 1, Defense(10));

        moves[4] = Move("Fuck You", 200, 0, 300000000,
                        std::vector<Status>{Burning(100), Shock(100), Bleed(100), Weakness(100)});
    }

    Move action() {
        int moveIdx = baseAction();
        if (moveIdx < 0) {
            return Move();
        }

        std::cout << moveIdx << '\n';

        static std::uniform_int_distribution<int> fuckYou =
            std::uniform_int_distribution<int>(0, 1000);
        if (fuckYou(seed) == 1000) {
            return moves[4];
        }

        if (moves[moveIdx].cost > stamina || moves[moveIdx].name == "Rest") {
            stamina += 5;
            if (stamina > maxStamina) stamina = maxStamina;
            return moves[0];
        } else {
            stamina -= moves[moveIdx].cost;
            if (moves[moveIdx].name == "Kick") {
                return moves[1];

            } else if (moves[moveIdx].name == "Block") {
                status.push_back(moves[3].status[0]);
                return moves[3];
            } else if (moves[moveIdx].name == "Observe") {
                return moves[2];
            }
        }
    }
};

#endif
