#include <iostream>
#include <cmath>
#include <imgui-SFML.h>

#include "Simulation.h"

int main() {

    std::srand(static_cast<unsigned>(std::time(NULL)));

    Simulation sim;

    while(sim.running()) {
        sim.update();
        sim.render();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
