#include "enemy.hpp"
#include <cstddef>
#include <iostream>
#include <string>

#ifndef WINDOW
#define WINDOW
class Window {

  public:
    Window() {
        mainWindow =
            "╔══════════════════════════════════════════════════════════════════════════════╗\n\
║\x1b[31m███▄▄▄▄    ▄██████▄       ███▄▄▄▄      ▄████████   ▄▄▄▄███▄▄▄▄      ▄████████ \x1b[0m║\n\
║\x1b[31m███▀▀▀██▄ ███    ███      ███▀▀▀██▄   ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███ \x1b[0m║\n\
║\x1b[31m███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    █▀  \x1b[0m║\n\
║\x1b[31m███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███  ▄███▄▄▄     \x1b[0m║\n\
║\x1b[31m███   ███ ███    ███      ███   ███ ▀███████████ ███   ███   ███ ▀▀███▀▀▀     \x1b[0m║\n\
║\x1b[31m███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    █▄  \x1b[0m║\n\
║\x1b[31m███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    ███ \x1b[0m║\n\
║\x1b[31m ▀█   █▀   ▀██████▀        ▀█   █▀    ███    █▀   ▀█   ███   █▀    ██████████ \x1b[0m║\n\
║                                                                              \x1b[0m║\n\
║\x1b[31m                      ▄████████    ▄███████▄    ▄██████▄                      \x1b[0m║\n\
║\x1b[31m                      ███    ███   ███    ███   ███    ███                    \x1b[0m║\n\
║\x1b[31m                      ███    ███   ███    ███   ███    █▀                     \x1b[0m║\n\
║\x1b[31m                     ▄███▄▄▄▄██▀   ███    ███  ▄███                           \x1b[0m║\n\
║\x1b[31m                    ▀▀███▀▀▀▀▀   ▀█████████▀  ▀▀███ ████▄                     \x1b[0m║\n\
║\x1b[31m                    ▀███████████   ███          ███    ███                    \x1b[0m║\n\
║\x1b[31m                      ███    ███   ███          ███    ███                    \x1b[0m║\n\
║\x1b[31m                      ███    ███  ▄████▀        ████████▀                     \x1b[0m║\n\
║\x1b[31m                      ███    ███                                              \x1b[0m║\n\
║\x1b[31m                                                                              \x1b[0m║\n\
╚══════════════════════════════════════════════════════════════════════════════╝";
        clearScreen();
        std::cout << mainWindow << '\n' << '\n';
        std::cout << "               1.) Start game\n               2.) Quit\n               3.) "
                     "Credits\n";
    }

    static void clearScreen() {
        // Safely clears the screen using ansi escape characters.
        // using system('cls') is pretty frowned upon
        std::cout << "\x1b[2J\x1b[1;1H" << std::flush;
    }

    void draw(std::string element) {
        std::cout
            << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
        int w = width(element);
    }

  protected:
    std::string mainWindow;

    /*
     * @brief: Finds the width of a given element.
     * Example:
     *  aaaa
     * aaaaaa
     *  aaaa
     * This string would return 6.
     * @param element: The string we want to measure.
     * @return int: The width of the element, if the element does not fit, it returns -1.
     */
    int width(std::string element) {
        int w = 0;
        for (char c : element) {
            if (c == '\n')
                return w;
            w++;
        }
        return w;
    }
};

class BattleWindow : public Window {};

class WeaponWindow : public Window {};

#endif
