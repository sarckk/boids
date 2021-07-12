#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "SpatialHashGrid.h"

//static void verify( std::map<int, std::unordered_set<int>>& t) {
//    for (auto const& x : t)
//    {
//        std::cout << x.first  // string (key)
//                  << ':'
//                  << x.second.size() // string's value
//                  << std::endl;
//    }
//}

SpatialHashGrid::SpatialHashGrid(const sf::Vector2u &winDim, int cellSize)
: m_windowDimensions(winDim)
, m_cellSize(cellSize)
{
    m_gridDimensions.x = ceil(winDim.x / cellSize);
    m_gridDimensions.y = ceil(winDim.y / cellSize);

    // initialise distance matrix here
    sf::Vector2u center = m_gridDimensions / (unsigned) 2;  // (0,0)
    int centerKey = createKey(center.x, center.y);

    // our m_grid representation is m_gridDimensions.x by m_gridDimensions.y
    std::map<int, std::unordered_set<int>> offsetsByDistToCenter {};
    int maxDistSquared = 0;

    for(int rowIndex = 0; rowIndex < m_gridDimensions.y; rowIndex++) {
        for(int colIndex = 0; colIndex < m_gridDimensions.x; colIndex++) {
            if(rowIndex == center.y && colIndex == center.x) {
                // this is the center cell
                offsetsByDistToCenter[0].insert(0);
            }
            int key = createKey(colIndex, rowIndex);
            int d_x = (abs(int(colIndex  - center.x))-1); // min.horizontal dist between this cell to center cell
            int d_y = (abs(int(rowIndex - center.y))-1); // min.vertical dist between this cell to center cell
            int distSquared = d_x * d_x + d_y * d_y;
            offsetsByDistToCenter[distSquared].insert(key - centerKey );

            if(distSquared > maxDistSquared)  maxDistSquared = distSquared; // record max squaredDist
        }
    }

    m_nOffsetsWithinSqDist = std::vector<int>{ maxDistSquared + 1 };
    int numCells = m_gridDimensions.x * m_gridDimensions.y;
    m_globalOffset = std::vector<int>{ numCells };

    int i = 0;

    // d2 stands for distance squared
    for(int d2 = 0; d2 <= maxDistSquared; d2++) {
        if(offsetsByDistToCenter[d2].empty()) {
            // if there are no cells whose min. squared dist. to center is d2
            m_nOffsetsWithinSqDist[d2] = m_nOffsetsWithinSqDist[d2-1];
        } else {
            for(int offset : offsetsByDistToCenter[d2]) {
                m_globalOffset[i] = offset;
                i++;
            }
            m_nOffsetsWithinSqDist[d2] = i;
        }
    }
}

void SpatialHashGrid::addBoid(Boid* boid) {
    // position of the base of boid arrow
    auto i1 = getIndices(boid->getPosition());
    int idx = createKey(i1.x, i1.y);
    m_grid[idx].insert(boid);

    boid->p_spatialIndex = idx;

    // position of the boid is at the center of the base of the arrow
    // position of the arrow tip
//    sf::Vector2f arrowTipPos = boid.getPosition() + boid.getDirection() * boid.getWidth();
//    auto i2 = getIndices(arrowTipPos);
//
//    for (int x = std::min(i1.first, i2.first); x <= std::max(i1.first, i2.first); ++x) {
//        for (int y = std::min(i1.second, i2.second); y <= std::max(i1.second, i2.second); ++y) {
//            m_grid[createKey(x, y)].push_back(boid);
//        }
//    }
}

// https://stackoverflow.com/questions/32685540/why-cant-i-compile-an-unordered-map-with-a-pair-as-key
// return (size_t) x << 32 | (unsigned int) y;

int SpatialHashGrid::createKey(int x, int y) {
    return x + y * m_gridDimensions.x;
}

sf::Vector2i SpatialHashGrid::getIndices(sf::Vector2f p) {
    float x = p.x / m_windowDimensions.x;
    float y = p.y / m_windowDimensions.y;

    int xIndex = floor(x * (m_gridDimensions.x - 1));
    int yIndex = floor(y * (m_gridDimensions.y - 1));

    return {xIndex, yIndex};
}

void SpatialHashGrid::clear() {
    m_grid.clear();
}

std::vector<Boid*> SpatialHashGrid::radiusSearch(const Boid* query, int radius) {
    std::vector<Boid*> res;

//    int d = radius / (m_cellSize * m_cellSize); // convert radius in pixel space to grid space
//    int n = std::floor(d * d);
    int n = radius * radius;

    int cellIndexOfQuery = query->p_spatialIndex;

    std::cout << m_nOffsetsWithinSqDist[n] << '\n';

    for(int i = 0; i < m_nOffsetsWithinSqDist[n]; i++) {
        int offset = m_globalOffset[i];

        // since the offsets are taken relative to origin, these could be out of bounds, keep it within m_gridDimensions
        int maxPossibleOffset = m_gridDimensions.x * m_gridDimensions.y - 1;
        int offsetWithinBounds = std::min<int>(std::max(0, cellIndexOfQuery + offset), maxPossibleOffset);

        for(auto boid : m_grid[offsetWithinBounds]) {
            if(boid == query) continue; // ignore itself
            res.push_back(boid);
        }
    }

//    sf::Vector2f arrowTipPos = query.getPosition() + query.getDirection() * query.getWidth();
//    auto i2 = getIndices(arrowTipPos);
//
//    for (int x = std::min(i1.first, i2.first); x <= std::max(i1.first, i2.first); ++x) {
//        for (int y = std::min(i1.second, i2.second); y <= std::max(i1.second, i2.second); ++y) {
//            int key = createKey(x, y);
//
//            for(const Boid& boid : m_grid[key]) {
//                if(&boid == &query) continue; // ignore itself
//                std::cout << "here\n";
//                res.push_back(boid);
//            }
//        }
//    }

    return res;
}

void SpatialHashGrid::removeBoid(Boid *boid) {
    m_grid[boid->p_spatialIndex].erase(boid); // O(1)
}

void SpatialHashGrid::updateBoid(Boid *boid) {
    auto i1 = getIndices(boid->getPosition());
    int key = createKey(i1.x, i1.y);

    if(key == boid->p_spatialIndex) {
        return; // same index occupied, don't update
    }

    removeBoid(boid);
    addBoid(boid);
}

