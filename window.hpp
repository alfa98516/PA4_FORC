#include <algorithm>
#include <cstring>
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
     * @brief: This function was freaky at first, I was working with only print statements, it was
     * unreadable and very bad. Then i needed to write the other 2 functions and this approach
     * became convoluted and almost impossible. This new approach uses a matrix of strings.
     * The reason i don't use a matrix of type char is because the character ║ is more than one
     * char.
     * @param element: the element we want to place in the window.
     */
    void draw(std::string element) {
        std::shared_ptr<std::vector<int> > temp = width(element);
        std::vector<int> w = (temp == nullptr) ? std::vector<int>{-1} : *temp.get();
        for (int i : w) {
            std::cout << i << '\n';
        }
        int h = height(element);
        if (w[0] == -1 || h == -1) // element doesn't fit
            return;

        const int h1PlacementIndex = (HEIGHT - h) / 2; // on what line should we start printing?

        int padding = (WIDTH - w[0]) / 2; // where should we start printing on the current line?
        auto windPointer = fillWindow();
        auto& wind = *windPointer.get();

        int k = 0;
        int i = padding;
        char c;
        int hPlace = h1PlacementIndex;
        for (int j = 0; j < element.size();) {
            c = element[j];

            if (c == '\n') {
                hPlace++;
                k++;
                i = (WIDTH - w[0]) / 2;
                j++;
                continue;
            }

            wind[hPlace][i] = c;
            j++;
            i++;
        }

        for (auto v : wind) {
            for (auto c : v) {
                std::cout << c;
            }
            std::cout << '\n';
        }
    }

    void draw(std::string element1, std::string element2) {
        std::shared_ptr<std::vector<int> > temp1 = width(element1);
        std::vector<int> w1 = (temp1 == nullptr) ? std::vector<int>{-1} : *temp1.get();
        int h1 = height(element1);
        if (w1[0] == -1 || h1 == -1) // element doesn't fit
            return;
        std::shared_ptr<std::vector<int> > temp2 = width(element2);
        std::vector<int> w2 = (temp2 == nullptr) ? std::vector<int>{-1} : *temp2.get();
        int h2 = height(element2);
        if (w2[0] == -1 || h2 == -1) // element doesn't fit
            return;

        const int h1PlacementIndex = (HEIGHT - h1) / 2;
        const int h2PlacementIndex = (HEIGHT - h2) / 2;
        auto windPointer = fillWindow();
        auto& wind = *windPointer.get();
        int maxW1 = *std::max_element(w1.begin(), w1.end());
        int maxW2 = *std::max_element(w2.begin(), w2.end());
        int padding = (WIDTH - maxW1 - maxW2) / 3;

        int k1 = 0;
        int i = padding;
        char c;
        int h1Place = h1PlacementIndex;
        for (int j = 0; j < element1.size();) {
            c = element1[j];

            if (c == '\n') {
                h1Place++;
                k1++;
                i = padding + (maxW1 - w1[k1]) / 2;
                j++;
                continue;
            }

            wind[h1Place][i] = c;
            j++;
            i++;
        }

        int k2 = 0;
        int w2start = padding + maxW1 + padding;
        i = w2start;
        int h2place = h2PlacementIndex;
        for (int j = 0; j < element2.size();) {
            c = element2[j];

            if (c == '\n') {
                h2place++;
                k2++;
                i = w2start + (maxW2 - w2[k2]) / 2;
                j++;
                continue;
            }

            wind[h2place][i] = c;
            j++;
            i++;
        }

        for (auto v : wind) {
            for (auto c : v) {
                std::cout << c;
            }
            std::cout << '\n';
        }
    }

    void draw(std::string element1, std::string element2, std::string element3) {
        std::shared_ptr<std::vector<int> > temp1 = width(element1);
        std::vector<int> w1 = (temp1 == nullptr) ? std::vector<int>{-1} : *temp1.get();
        int h1 = height(element1);
        if (w1[0] == -1 || h1 == -1) // element doesn't fit
            return;

        std::shared_ptr<std::vector<int> > temp2 = width(element2);
        std::vector<int> w2 = (temp2 == nullptr) ? std::vector<int>{-1} : *temp2.get();
        int h2 = height(element2);
        if (w2[0] == -1 || h2 == -1) // element doesn't fit
            return;

        std::shared_ptr<std::vector<int> > temp3 = width(element3);
        std::vector<int> w3 = (temp3 == nullptr) ? std::vector<int>{-1} : *temp3.get();
        int h3 = height(element3);
        if (w3[0] == -1 || h3 == -1) // element doesn't fit
            return;

        const int h1PlacementIndex = (HEIGHT - h1) / 2;
        const int h2PlacementIndex = (HEIGHT - h2) / 2;
        const int h3PlacementIndex = (HEIGHT - h3) / 2;
        int maxW1 = *std::max_element(w1.begin(), w1.end());
        int maxW2 = *std::max_element(w2.begin(), w2.end());
        int maxW3 = *std::max_element(w3.begin(), w3.end());
        int padding = (WIDTH - maxW1 - maxW2 - maxW3) / 4;

        auto windPointer = fillWindow();
        auto& wind = *windPointer.get();

        int h1Place = h1PlacementIndex;
        char c;
        int k1 = 0;
        int i = padding;
        for (int j = 0; j < element1.size();) {
            c = element1[j];
            if (c == '\n') {
                h1Place++;
                k1++;
                i = padding + (maxW1 - w1[k1]) / 2;
                j++;
                continue;
            }
            wind[h1Place][i] = c;
            j++;
            i++;
        }

        int h2place = h2PlacementIndex;
        int k2 = 0;
        int w2Start = padding + maxW1 + padding;
        i = w2Start;
        for (int j = 0; j < element2.size();) {
            c = element2[j];
            if (c == '\n') {
                h2place++;
                k2++;
                i = w2Start + (maxW2 - w2[k2]) / 2;
                j++;
                continue;
            }
            wind[h2place][i] = c;
            j++;
            i++;
        }

        int h3place = h3PlacementIndex;
        int k3 = 0;
        int w3Start = padding + maxW1 + padding + maxW2 + padding;
        i = w3Start;
        for (int j = 0; j < element3.size();) {
            c = element3[j];
            if (c == '\n') {
                h3place++;
                k3++;
                i = w3Start + (maxW3 - w3[k3]) / 2;
                j++;
                continue;
            }
            wind[h3place][i] = c;
            j++;
            i++;
        }

        for (auto v : wind) {
            for (auto ch : v) {
                for (auto c : ch) {
                    std::cout << c;
                }
            }
            std::cout << '\n';
        }
    }

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
        for (char ch : element) {
            unsigned char c = static_cast<unsigned char>(ch);
            ;
            if (c == '\n') {
                w->push_back(currW);
                if (currW > maxW)
                    maxW = currW;
                currW = 0;
            } else if (c < 0x80 || c >= 0xc0) {
                currW++;
            }
        }
        return (maxW <= WIDTH) ? w : nullptr;
    }

    std::shared_ptr<std::vector<std::vector<std::string> > > fillWindow() {
        auto wind = std::make_shared<std::vector<std::vector<std::string> > >(
            HEIGHT + 2, std::vector<std::string>(WIDTH + 1)); // 21 × 82
        auto& w = *wind.get();
        for (int i = 0; i < HEIGHT + 2; ++i) {
            for (int j = 0; j < WIDTH + 1; ++j) {
                if ((i == 0 || i == HEIGHT + 1) && (j > 0 && j < WIDTH)) {
                    w[i][j] = "═";
                } else if (i == 0 && j == 0) {
                    w[0][0] = "╔";
                } else if (i == 0 && j == WIDTH) {
                    w[i][j] = "╗";
                } else if (i == HEIGHT + 1 && j == 0) {
                    w[i][j] = "╚";
                } else if (i == HEIGHT + 1 && j == WIDTH) {
                    w[i][j] = "╝";
                } else if ((i > 0 && i < HEIGHT + 1) && (j == 0 || j == WIDTH)) {
                    w[i][j] = "║";
                } else {
                    w[i][j] = " ";
                }
            }
        }
        return wind;
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
