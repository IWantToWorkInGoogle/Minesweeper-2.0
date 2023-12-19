#include "plain.h"


int main() {
    Level level;
    int type;
    std::cout <<    "Levels:\n" <<
                    "1.Easy(9x9,10 mines)\n" <<
                    "2.Normal(16x16,40 mines)\n" <<
                    "3.Hard(16x30,99 mines)\n" <<
                    "4.Custom(not working)\n"
                    "Choose: ";
    std::cin >> type;
    switch (type) {
        case 1:
            level = Level::EASY;
            break;
        case 2:
            level = Level::NORMAL;
            break;
        case 3:
            level = Level::HARD;
            break;
        case 4:
            level = Level::CUSTOM;
            break;
        default:
            std::cout << "Choosed invalid value\n";
            return 0;
    }

    auto game = SinglePlayer(level);
    game.run();
    return 0;
}
