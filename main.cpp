#include "Game.h"
#include <stdexcept>
#include <iostream>

int main() {
    try {

        Game Terris;

        Terris.init();

        Terris.run();

    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "..." << std::endl;
    }
    return 0;
}
