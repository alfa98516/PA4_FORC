#include "enemy.hpp"
#include "window.hpp"
#include <fstream>
#include <iostream>
#include <string>
int main() {
    Window w = Window();
    std::string a = "";
    std::ifstream f("./sprites/frogtest.txt");
    std::string s;
    while (getline(f, s))
        a += s + '\n';
    f.close();
    std::cout << "Select: ";
    int option;
    std::cin >> option;
    w.clearScreen();
    w.draw(a, a, a);
    std::cout << "Select: ";
    std::cin >> option;
}
