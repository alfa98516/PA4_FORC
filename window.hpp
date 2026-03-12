#include "enemy.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
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
        std::shared_ptr<std::vector<int> > temp = width(element);
        std::vector<int> w = (temp == nullptr) ? std::vector<int>{-1} : *temp.get();
        int h = height(element);
        if (w[0] == -1 || h == -1) // element doesn't fit
            return;
        int hCount = 0;
        int wCount = 0;
        int wPlacementIdx =
            (WIDTH - w[0]) / 2; // where should we start printing on the current line?
        const int hPlacementIdx = (HEIGHT - h) / 2; // on what line can we start printing?

        // add padding for the element.
        while (hCount < hPlacementIdx) {

            std::cout << "║                                                                    "
                         "          ║\n";
            hCount++;
        }
        char c;
        int j = 0;
        int i = 0;
        int wPlacementCount = 0;
        for (int i = 0; i < element.length();) {
            c = element[i];
            wPlacementIdx = (WIDTH - w[j]) / 2;
            if (c == '\n') {
                while (wPlacementCount <
                       WIDTH - 2) { // removes the two ║ characters out of the count.
                    std::cout << " ";
                    wPlacementCount++;
                }
                wPlacementCount = 0;
                wCount = 0;
                c = element[++i];
                j++;
                std::cout << "║\n";
            }

            if (wCount == 0 && c != '\n') {
                std::cout << "║";
            }

            while (wCount < wPlacementIdx && c != '\n') {
                std::cout << " ";
                wCount++;
                wPlacementCount++;
            }
            if (c != '\n') {
                std::cout << c;
                wPlacementCount++;
                i++;
            }
        }

        while (wPlacementCount < WIDTH - 1) { // I have no idea why - 1 works here but it does and
                                              // im not clambering to figure out why.
            std::cout << " ";
            wPlacementCount++;
        }

        // int k = 0;
        // while (k < wPlacementIdx - 1) { // when on the last character, it wont add trailing
        // spaces,
        //                                 // since the for loop ends before it can attempt.
        //     std::cout << " ";
        //     k++;
        // }

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

    void draw(std::string element1, std::string element2) {
        std::shared_ptr<std::vector<int> > temp1 = width(element1);
        std::vector<int> w1 = (temp1 == nullptr) ? std::vector<int>{-1} : *temp1.get();
        int h1 = height(element1);
        if (w1[0] == -1 || h1 == -1) // element doesn't fit
            return;
        int h1Count = 0;
        int w1Count = 0;
        std::shared_ptr<std::vector<int> > temp2 = width(element2);
        std::vector<int> w2 = (temp2 == nullptr) ? std::vector<int>{-1} : *temp2.get();
        int h2 = height(element2);
        if (w2[0] == -1 || h2 == -1) // element doesn't fit
            return;
        int h2Count = 0;
        int w2Count = 0;

        const int h1PlacementIndex = (HEIGHT - h1) / 2;
        const int h2PlacementIndex = (HEIGHT - h2) / 2;

        std::vector<std::vector<std::string> > wind =
            std::vector<std::vector<std::string> >(20, std::vector<std::string>(80));

        // fill window in
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 80; ++j) {
                if ((i == 0 || i == 19) && ((j > 0 && j < 79) || (j > 0 && j < 79))) {
                    wind[i][j] = "═";
                } else if (i == 0 && j == 0) {
                    wind[0][0] = "╔";
                } else if (i == 0 && j == 79) {
                    wind[i][j] = "╗";
                } else if (i == 19 && j == 0) {
                    wind[i][j] = "╚";
                } else if (i == 19 && j == 79) {
                    wind[i][j] = "╝";
                } else if ((i != 0 && i != 79) && (j == 0 || j == 79)) {
                    wind[i][j] = "║";
                } else {
                    wind[i][j] = " ";
                }
            }

            for (int i = 0; i < element1.size();) {
            }

            for (std::vector<std::string> v : wind) {
                for (std::string c : v) {
                    std::cout << c;
                }
                std::cout << '\n';
            }
        }
    }
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
    std::shared_ptr<std::vector<int> > width(std::string element) {
        int currW = 0;
        int maxW = 0;
        std::shared_ptr<std::vector<int> > w = std::make_shared<std::vector<int> >();
        for (char c : element) {
            if (c == '\n') {
                w->push_back(currW);
                if (currW > maxW) maxW = currW;
                currW = 0;
            }
            currW++;
        }

        return (maxW <= WIDTH) ? w : nullptr;
    }

    int height(std::string element) {
        int h = 0;
        for (char c : element) {
            if (c == '\n') h++;
        }
        return (h + 1 <= HEIGHT) ? h + 1 : -1;
    }
};

#endif
