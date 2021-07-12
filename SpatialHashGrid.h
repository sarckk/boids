#ifndef BOIDS_SPATIALHASHGRID_H
#define BOIDS_SPATIALHASHGRID_H

#include<unordered_set>

#include <SFML/System.hpp>

#include "Boid.h"

class SpatialHashGrid {
private:
    int m_cellSize;
    sf::Vector2u m_gridDimensions; // dim of the grid cells (e.g. 3 by 2)
    sf::Vector2u m_windowDimensions; // dim of the screen that grid covers (window in our case)
    std::map<int, std::unordered_set<Boid*>> m_grid;

    // numOffsetsWithinDist[i] contains no. of cells in the grid which are within squared distance <i> from center of grid (cumulative sum)
    std::vector<int> m_nOffsetsWithinSqDist; // maxSquaredDist + 1 in size
    std::vector<int> m_globalOffset; // numCells in size

    int createKey(int x, int y);
    sf::Vector2i getIndices(sf::Vector2f p);

public:
    SpatialHashGrid() = default;

    SpatialHashGrid(const sf::Vector2u& winDim, int cellSize);

    void addBoid(Boid* boid);
    void removeBoid(Boid* boid);
    void updateBoid(Boid* boid);
    void clear();
    std::vector<Boid*> radiusSearch(const Boid* query, int radius);
};

#endif //BOIDS_SPATIALHASHGRID_H
