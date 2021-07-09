#include <iostream>
#include <cmath>
#include "Simulation.h"

#include <imgui-SFML.h>

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
