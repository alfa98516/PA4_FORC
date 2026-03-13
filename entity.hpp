#include <algorithm>
#include <iostream>
#include <random>
#include <typeinfo>
#include <utility>
#include <vector>
#include "moves.hpp"
#include <memory>
#include<fstream>

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

    bool isShocked() {
        for (auto& st : status) {
            if (st->name == "Shock" && st->level > 0) return true;
        }
        return false;
    }

    void applyStatuses(){ statusManager();};
    Entity(const Entity& other)
        : health(other.health), maxHealth(other.maxHealth), stamina(other.stamina),
        maxStamina(other.maxStamina), name(other.name), art(other.art),
        dist(other.dist), defense(other.defense), attack(other.attack),
        allies(other.allies), moves(other.moves) {
        for (const auto& st : other.status) {
            status.push_back(st->clone());
        }
    }
  protected:
    virtual int baseAction() {
        if (isShocked()) {
            return -1;
        }

        int moveIdx = dist(seed);

        return moveIdx;
    }

    std::string getSprite(std::string filename) {
        std::string sprite = "";
        std::ifstream f("./sprites/" + filename);
        std::string line;
        while (getline(f,line)) sprite += line + '\n';
        f.close();
        return sprite;
    }


  protected:
    void statusManager() {
        defense = 0;
        attack = 1;
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
        moves[0] = Move("Rest", 0, 0, 0, "Restore 5 stamina");
        allies = std::make_pair(nullptr, nullptr);
    }
};

class Player : public Entity{
    public:
        explicit Player(std::string _name) : Entity(_name, 100, 20, 0, 3){
            moves[1] = Move("Punch", 20, 0, 4, "Deal 20 damage");
            moves[2] = Move("Block", 0, 0, 2, "Block 10 incoming damage", Defense::create(10, true));
            moves[3] = Move("Heal",  0, 40, 6, "Heal 40 HP");
        }
        
        Move action() override {
            if (isShocked()) {
                std::cout << name <<" are shocked so you do nothing\n";
                return Move();
            }
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
                    continue;
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
        moves[1] = Move("Kick", 10, 0, 2, "Deal 10");
        moves[2] = Move("Observe", 0, 0, 4, "Add 10 weakness to enemy", Weakness::create(10, false));
        moves[3] = Move("Block", 0, 0, 1, "Take 10 less damage this turn", Defense::create(10, true));

        moves[4] = Move("Fuck You", 2000000, 0, 300000000,"FUCK YOU",
                        std::vector<std::shared_ptr<Status>>{Burning::create(100, false), Shock::create(100, false), Bleed::create(100, false), Weakness::create(100, false)});
        art = getSprite("skitter1.txt");
    }

    Move action() {
        if (isShocked()) {
            std::cout << name <<" are shocked so you do nothing\n";
            return Move();
        }
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
            moves[1] = Move("Sword Slice", 5, 0, 4, "", Bleed::create(3, false));
            moves[2] = Move("Axe Cleave", 20, 0, 6, "");
            moves[3] = Move("Enrage", 0, 0, 5, "", std::vector<std::shared_ptr<Status>>{Strength::create(3, true), Weakness::create(10, true)});
            //moves[4] = Move("Double Strike") Maybe???

            art = getSprite("lille_giant.txt");
        }
    
        Move action() {
            if (isShocked()) {
                std::cout << name <<" are shocked so you do nothing\n";
                return Move();
            }
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
            moves[1] = Move("Slice", 20, 0, 4, "", Bleed::create(3, false));
            moves[2] = Move("Soul sucker",  10, 5, 4, "");
            moves[3] = Move("Invisible", 0, 0, 6, "", Invisible::create(2,true));

            art = getSprite("dauhi.txt");
        }

        Move action() {
            if (isShocked()) {
                std::cout << name <<" are shocked so you do nothing\n";
                return Move();
            }
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

class Hydra1 : public Entity {
    public:
        explicit Hydra1() : Entity("Hydra", 100, 20, 3, 3) {
            moves[1] = Move("Fire cough", 5, 0, 4, "", Burning::create(1, false));
            moves[2] = Move("Stomp", 20, 0, 6, "");
            moves[3] = Move("Fire ring", 10, 0, 10, "", std::vector<std::shared_ptr<Status>>{Defense::create(10, true), Burning::create(1, false)});
            art = getSprite("hydra1.txt");
        }

        Move action() {
            if (isShocked()) {
                std::cout << name << " are shocked so you do nothing\n";
                return Move();
            }
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

class Hydra2 : public Entity {
    public:
        explicit Hydra2() : Entity("Hydra", 300, 30, 4, 4) {
            moves[1] = Move("Fire Blast", 15, 0, 10, "", Burning::create(2, false));
            moves[2] = Move("Electric Breath", 10, 0, 10, "", Shock::create(1, false));
            moves[3] = Move("Decay", 0, 0, 10, "", Weakness::create(10, false));
            moves[4] = Move("Element rings", 10, 0, 30, "", std::vector<std::shared_ptr<Status>> {Defense::create(10, true), Burning::create(1, false), Weakness::create(10,false), Shock::create(1,false)});
            art = getSprite("hydra2.txt");
        }

        
        Move action() {
            if (isShocked()) {
                std::cout << name <<" are shocked so you do nothing\n";
                return Move();
            }
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