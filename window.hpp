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
            "╔═══════════════════════════════════════════════════════════════════════════════╗\n\
║\x1b[31m ███▄▄▄▄    ▄██████▄       ███▄▄▄▄      ▄████████   ▄▄▄▄███▄▄▄▄      ▄████████ \x1b[0m║\n\
║\x1b[31m ███▀▀▀██▄ ███    ███      ███▀▀▀██▄   ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███ \x1b[0m║\n\
║\x1b[31m ███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    █▀  \x1b[0m║\n\
║\x1b[31m ███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███  ▄███▄▄▄     \x1b[0m║\n\
║\x1b[31m ███   ███ ███    ███      ███   ███ ▀███████████ ███   ███   ███ ▀▀███▀▀▀     \x1b[0m║\n\
║\x1b[31m ███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    █▄  \x1b[0m║\n\
║\x1b[31m ███   ███ ███    ███      ███   ███   ███    ███ ███   ███   ███   ███    ███ \x1b[0m║\n\
║\x1b[31m  ▀█   █▀   ▀██████▀        ▀█   █▀    ███    █▀   ▀█   ███   █▀    ██████████ \x1b[0m║\n\
║                                                                               \x1b[0m║\n\
║\x1b[31m                       ▄████████    ▄███████▄    ▄██████▄                      \x1b[0m║\n\
║\x1b[31m                       ███    ███   ███    ███   ███    ███                    \x1b[0m║\n\
║\x1b[31m                       ███    ███   ███    ███   ███    █▀                     \x1b[0m║\n\
║\x1b[31m                      ▄███▄▄▄▄██▀   ███    ███  ▄███                           \x1b[0m║\n\
║\x1b[31m                     ▀▀███▀▀▀▀▀   ▀█████████▀  ▀▀███ ████▄                     \x1b[0m║\n\
║\x1b[31m                     ▀███████████   ███          ███    ███                    \x1b[0m║\n\
║\x1b[31m                       ███    ███   ███          ███    ███                    \x1b[0m║\n\
║\x1b[31m                       ███    ███  ▄████▀        ████████▀                     \x1b[0m║\n\
║\x1b[31m                       ███    ███                                              \x1b[0m║\n\
║\x1b[31m                                                                               \x1b[0m║\n\
╚═══════════════════════════════════════════════════════════════════════════════╝";
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
    /*
     * @brief: Freaky magic function, I wrote it and I barely know how I did it.
     *         There are so many potential off by one errors.
     * @param element: the element we want to place in the window.
     */

    void draw(std::string element) {
        std::cout
            << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
        int w = width(element);
        int h = height(element);
        if (w == -1 || h == -1) // element doesn't fit
            return;
        int hCount = 0;
        int wCount = 0;
        const int wPlacementIdx =
            (WIDTH - w) / 2; // where should we start printing on the current line?
        const int hPlacementIdx = (HEIGHT - h) / 2; // on what line can we start printing?

        // add padding for the element.
        while (hCount < hPlacementIdx) {

            std::cout << "║                                                                    "
                         "          ║\n";
            hCount++;
        }
        char c;
        int i = 0;
        for (int i = 0; i < element.length();) { // I ommit the increment here because we
                                                 // dont always want to increment.
            c = element[i];

            if (c == '\n') { // Here I'm adding the trailing spaces behind the the element.
                int j = 0;

                while (j <
                       wPlacementIdx - 1) { // wPlacementIdx coincidentaly works here because its
                    // technically just how much padding to add to the element
                    std::cout << " ";
                    j++;
                }
                std::cout << "║\n";
                wCount = 0;
                c = element[++i];
            }

            if (wCount == 0) // start of the line
                std::cout << "║";

            while (wCount < wPlacementIdx) { // add padding until we can print a character
                std::cout << " ";
                wCount++;
            }
            if (wCount >= wPlacementIdx) {
                std::cout << c;
                wCount++;
                i++;
            }
        }

        int j = 0;
        while (j < wPlacementIdx - 1) { // when on the last character, it wont add trailing spaces,
                                        // since the for loop ends before it can attempt.

            std::cout << " ";
            j++;
        }

        std::cout << "║\n";
        int count = 0;
        while (count < hPlacementIdx) {
            std::cout << "║                                                                    "
                         "          ║\n";
            count++;
        }
        if (hPlacementIdx * 2 + h < HEIGHT) { // fixes an off by one error.
            std::cout << "║                                                                    "
                         "          ║\n";
        }

        std::cout << "╚════════════════════════════════════════════════════════════════════════"
                     "══════╝\n";
    }

    void draw(std::string element1, std::string element2) {}
    void draw(std::string element1, std::string element2, std::string element3) {}

  protected:
    std::string mainWindow;
    const int HEIGHT = 19;
    const int WIDTH = 80;

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
        return (w <= WIDTH) ? w : -1;
    }

    int height(std::string element) {
        int h = 0;
        for (char c : element) {
            if (c == '\n')
                h++;
        }
        return (h + 1 <= HEIGHT) ? h + 1 : -1;
    }
};

#endif
