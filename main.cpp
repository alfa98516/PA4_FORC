#include "enemy.hpp"
#include "window.hpp"
#include <iostream>
#include <string>
int main() {
    Window w = Window();
    std::string a = " aaa \n\
aaaaa\n\
aaaaa\n\
 aaa ";
    int option;
    std::cout << "Select: ";
    std::cin >> option;
    w.clearScreen();
    w.draw(a);
    std::cout << "Select: ";
    std::cin >> option;
}
