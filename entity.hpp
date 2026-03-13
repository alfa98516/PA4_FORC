#include <algorithm>
#include <iostream>
#include <random>
#include <typeinfo>
#include <utility>
#include <vector>
#include "moves.hpp"
#include <memory>

#ifndef ENTITY
#define ENTITY
class Entity {
  public:
    std::random_device seed = std::random_device{
        "rdrand"}; // Looks for a device connected to the computer that can generate
                   // non-deterministic, true random numbers. Pretty intensive
                   // (around 300 clock cycles). We use this as the seed.
    virtual ~Entity() = default;
    std::vector<Move> moves;
    int health;
    const int maxHealth;
    std::uniform_int_distribution<int> dist;
    int stamina;
    const int maxStamina;
    std::string name;
    std::string art;
    std::vector<std::shared_ptr<Status>> status;
    int defense;
    int attack;
    bool shocked = false;
    std::pair<Entity*, Entity*> allies;
    virtual Move action() { return Move();}
    void updateStatus(std::shared_ptr<Status> st) {
        auto it = std::find_if(status.begin(), status.end(), [&](const std::shared_ptr<Status>& s){ return s->name == st->name; });
        if(it != status.end()){
            std::cout << "Stacking " << st->name << ": level was " << (*it)->level << " adding " << st->level << "\n";
            (*it)->level += st->level;
            if (st->name == "Burning" || st->name == "Bleed"){
                (*it)->updateDmg();
            }
        } else {
            status.push_back(st->clone());
        }
    }
    void applyStatuses(){ statusManager();};
  protected:
    virtual int baseAction() {
        if (shocked) {
            return -1;
        }

        int moveIdx = dist(seed);

        return moveIdx;
    }


  protected:
    void statusManager() {
        defense = 0;
        attack = 1;
        shocked = false;
        for (auto it = status.begin(); it != status.end();) {
            if (((*it)->name == "Burning" || (*it)->name == "Bleed") && (*it)->level > 0) {
                health -= (*it)->getDmg();
                std::cout << name << " Took " << (*it)->getDmg() << " damage from " << (*it)->name << "\n" ;
            } else if (((*it)->name == "Defense" || (*it)->name == "Weakness") && (*it)->level > 0) {
                defense += (*it)->getBlock();
                std::cout << name << " Got " << (*it)->getBlock() << " defense from " << (*it)->name << "\n";
            } else if ((*it)->name == "Strength" && (*it)->level) {
                attack = (*it)->getScaler();
                std::cout << name << " Got extra " << (*it)->getScaler() << "* Damage from " << (*it)->name << "\n";
            } else if ((*it)->name == "Shock" && (*it)->level) {
                shocked = true;
                std::cout << name << " is shocked for " << (*it)->level << " turns\n";
            }

            (*it)->decreaseLevel();

            if ((*it)->level <= 0) {
                std::cout << (*it)->name << " has worn off for " << name << "\n";
                it = status.erase(it);
            } else {
                ++it;
            }
        }
    }


    Entity(std::string _name, int _health, int _stamina, int b, int nMoves)
        : health(_health), stamina(_stamina), name(_name), dist(0, b), moves(nMoves + 1),
          defense(0), attack(1), maxStamina(_stamina), maxHealth(_health) {
        moves[0] = Move("Rest", 0, 0, 0);
        allies = std::make_pair(nullptr, nullptr);
    }
};

class Player : public Entity{
    public:
        explicit Player(std::string _name) : Entity(_name, 100, 20, 0, 3){
            moves[1] = Move("Punch", 20, 0, 4);
            moves[2] = Move("Block", 0,  0, 2, Defense::create(10, true));
            moves[3] = Move("Heal",  0,  40, 6);
        }
        
        Move action() override {
            int i = 1;
            for (Move move : moves) {
                std::cout << i << "  " << move.name << " Cost: " << move.cost << "\n";
                i++;
            }
            int choice;
            bool done = false;
            Move move;
            while (!done){
                std::cout << "Pick a move: ";
                std::cin >> choice;
                if (choice < 1 || choice > moves.size()){
                    std::cout << "Not a viable move" << "\n";
                }
                move = moves[choice-1];
                if(stamina < move.cost){
                    std::cout << "You don't have enough stamina for this move, please rest first" << "\n";
                    continue;
                }
                done = true;
            }
            return move;
        }
};
class Minion : public Entity {
  public:
    explicit Minion() : Entity("Minion", 50, 10, 3, 4) {
        moves[1] = Move("Kick", 10, 0, 2);
        moves[2] = Move("Observe", 0, 0, 4, Weakness::create(10, false));
        moves[3] = Move("Block", 0, 0, 1, Defense::create(10, true));

        moves[4] = Move("Fuck You", 2000000, 0, 300000000,
                        std::vector<std::shared_ptr<Status>>{Burning::create(100, false), Shock::create(100, false), Bleed::create(100, false), Weakness::create(100, false)});
    }

    Move action() {
        int moveIdx = baseAction();
        if (moveIdx < 0) {
            return Move();
        }

        static std::uniform_int_distribution<int> fuckYou =
            std::uniform_int_distribution<int>(0, 1000);
        if (fuckYou(seed) == 1000) {
            return moves[4];
        }

        if (moves[moveIdx].cost > stamina || moves[moveIdx].name == "Rest") {
            stamina += 5;
            if (stamina > maxStamina) stamina = maxStamina;
        } 
        return moves[moveIdx];
    }
};

class TwoHeadedGiant : public Entity {
    public:
        explicit TwoHeadedGiant() : Entity("Two Headed Giant", 100, 20, 3, 4) {
            moves[1] = Move("Sword Slice", 20, 0, 4, Bleed::create(5, false));
            moves[2] = Move("Axe Cleave", 40, 0, 6);
            moves[3] = Move("Enrage", 0, 0, 5, std::vector<std::shared_ptr<Status>>{Strength::create(3, true), Weakness::create(10, true)});
            //moves[4] = Move("Double Strike") Maybe???
        }
    
    Move action() {
        int moveIdx = baseAction();
        if (moveIdx < 0) {
            return Move();
        }

        if (moves[moveIdx].cost > stamina || moves[moveIdx].name == "Rest") {
            stamina += 5;
            if (stamina > maxStamina) stamina = maxStamina;
        } 
        return moves[moveIdx];
    }
};

class Dauthi : public Entity {
    public:
        explicit Dauthi() : Entity("Dauthi", 200, 20, 3, 3) {
            moves[1] = Move("Slice", 20, 0, 4, Bleed::create(5, false));
            moves[2] = Move("Soul sucker", 10, 10, 4);
            moves[3] = Move("Invisible", 0, 0, 6, Invisible::create(2,true));
        }

        Move action() {
            int moveIdx = baseAction();
            if (moveIdx < 0) {
                return Move();
            }

            if (moves[moveIdx].cost > stamina || moves[moveIdx].name == "Rest") {
                stamina += 5;
                if (stamina > maxStamina) stamina = maxStamina;
            } 
            return moves[moveIdx];
        }
};
#endif