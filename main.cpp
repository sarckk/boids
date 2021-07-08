#include <iostream>
#include <cmath>
#include "Simulation.h"
#include "Vector2d.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(NULL)));

    Simulation sim;

    while(sim.running()) {
        sim.update();
        sim.render();
    }
//    Vector2d a = {-394, 2000};
//    a.x = fmod(a.x, 1920);
//    a.y = fmod(a.y, 1080);
//    if(a.x < 0) a.x += 1920;
//    if(a.y < 0) a.x += 1080;
//
//    std::cout << a << '\n';

    return 0;
}
