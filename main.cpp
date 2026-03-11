#include "enemy.hpp"
#include "window.hpp"
#include <iostream>
int main() {
    Minion auddi = Minion();
    Move act = auddi.action();
    std::cout << act.name << '\n';
}
