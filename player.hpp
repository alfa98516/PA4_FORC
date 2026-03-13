#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <utility>
#include <vector>
#include "moves.hpp"

#ifndef PLAYER
#define PLAYER
class Player {
    public:
        std::string name;
        int health;
        const int maxHealth;
        int stamina;
        const int maxStamina;
        int defense;
        std::vector<Move> moves;
        std::vector<Status> status;
        bool shocked = false;

        Player(std::string _name) : name(_name), health(100), maxHealth(100), stamina(20), maxStamina(20), defense(0){
            moves.push_back(Move("Rest",  0,  0, 0));
            moves.push_back(Move("Punch", 20, 0, 4));
            moves.push_back(Move("Block", 0,  0, 2, Defense(10, true)));
            moves.push_back(Move("Heal",  0,  40, 6));
        }
    
    
};
#endif
