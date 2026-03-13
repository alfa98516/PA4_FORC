#include "entity.hpp"
#include <random>
#include <iostream>
bool quit = false;

int main() {
    std::string name;
    std::cout << "Input your name: ";
    std::cin >> name;
    Player player = Player(name);
    std::cout << "Welcome " << player.name << "\n";

    while(!quit) {
        TwoHeadedGiant minion = TwoHeadedGiant();
        std::cout << "A wild " << minion.name << " appears\n";

        while(minion.health >= 0) {
            std::cout << minion.name << "  Health: " << minion.health << "/" << minion.maxHealth << "\n";
            std::cout << player.name << "  Health: " << player.health << "/" << player.maxHealth << "  Stamina: " << player.stamina << "/" << player.maxStamina << "\n";
            Move move = player.action();

            std::cout << player.name << " used " << move.name << "\n";

            minion.health -= std::max(0, (move.attackPower * player.attack) - minion.defense);
            player.health += move.heal;
            player.stamina -= move.cost;
            if (player.health > player.maxHealth) player.health = player.maxHealth;
            if (move.name == "Rest") {
                player.stamina += 5;
                if (player.stamina > player.maxStamina) player.stamina = player.maxStamina;
            }
            if (!move.status.empty()) {
                for (auto& st : move.status) {
                    if( st->self) {
                        player.updateStatus(st);
                    } else {
                        minion.updateStatus(st);
                    }
                }
            }
            player.applyStatuses();
            if (minion.health <= 0) {
                std::cout << minion.name << " has been defeated!" << "\n";
                quit = true;
                break;
            }
            std::cout << minion.name << "  Health: " << minion.health << "/" << minion.maxHealth << "\n";
            std::cout << player.name << "  Health: " << player.health << "/" << player.maxHealth << "  Stamina: " << player.stamina << "/" << player.maxStamina << "\n";

            Move enemyMove = minion.action();

            std::cout << minion.name << " used " << enemyMove.name << "\n";

            player.health -= std::max(0, (enemyMove.attackPower * minion.attack) - player.defense);
            minion.health += enemyMove.heal;
            minion.stamina -= enemyMove.cost;
            if (minion.health > minion.maxHealth) minion.health = minion.maxHealth;
            if (!enemyMove.status.empty()) {
                for (auto& st : enemyMove.status) {
                    if(st->self) {
                        minion.updateStatus(st);
                    } else {
                        player.updateStatus(st);
                    }
                }
            }
            minion.applyStatuses();
            if (player.health <= 0) {
                std::cout << "YOU DIED" << "\n";
                quit = true;
                break;
            }
        }
    }
}
