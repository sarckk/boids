#include <cstdlib>
#include <algorithm>

#include "SpatialHashGrid.h"
#include "VectorArithmetic.h"

SpatialHashGrid::SpatialHashGrid(const sf::Vector2u &winDim, int cellSize)
: m_windowDimensions(winDim)
, m_cellSize(cellSize)
{
    m_gridDimensions.x = ceil(winDim.x / (double)cellSize);
    m_gridDimensions.y = ceil(winDim.y / (double)cellSize);

    // initialise distance matrix here
    sf::Vector2u center = m_gridDimensions / (unsigned) 2;  // considered as (0,0) for calculation of sq dist later
    int centerKey = createKey(center.x, center.y);

    // our m_grid representation is m_gridDimensions.x by m_gridDimensions.y
    std::map<int, std::unordered_set<int>> offsetsByDistToCenter {};
    int maxDistSquared = 0;

    for(int rowIndex = 0; rowIndex < m_gridDimensions.y; rowIndex++) {
        for(int colIndex = 0; colIndex < m_gridDimensions.x; colIndex++) {
            if(rowIndex == center.y && colIndex == center.x) {
                // this is the center cell
                offsetsByDistToCenter[0].insert(0);
                continue;
            }
            int key = createKey(colIndex, rowIndex);
            int d_x = std::max((abs(int(colIndex - center.x))-1),0); // min.horizontal dist between this cell to center cell
            int d_y = std::max((abs(int(rowIndex - center.y))-1),0); // min.vertical dist between this cell to center cell
            int distSquared = d_x * d_x + d_y * d_y;
            offsetsByDistToCenter[distSquared].insert(key - centerKey );

            if(distSquared > maxDistSquared)  maxDistSquared = distSquared; // record max squaredDist
        }
    }

    m_nOffsetsWithinSqDist = new int[maxDistSquared + 1];
    int numCells = m_gridDimensions.x * m_gridDimensions.y;
    m_globalOffset = new int[numCells];

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

void SpatialHashGrid::addBoid(std::shared_ptr<Boid> boid) {
    // position of the base of boid arrow
    auto i1 = getIndices(boid->getPosition());
    int idx = createKey(i1.x, i1.y);
    m_grid[idx].insert(boid);

    boid->p_spatialIndex = idx;
}

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

std::vector<std::shared_ptr<Boid>> SpatialHashGrid::radiusSearch(const std::shared_ptr<Boid> query, int radius) {
    using namespace VectorArithmetic;

    std::vector<std::shared_ptr<Boid>> res;

    double d = radius / static_cast<double>(m_cellSize); // convert radius in pixel space to grid space
    int n = std::floor(d * d);

    int cellIndexOfQuery = query->p_spatialIndex;

    for(int i = 0; i < m_nOffsetsWithinSqDist[n]; i++) {
        int offset = m_globalOffset[i];

        // since the offsets are taken relative to origin, these could be out of bounds, keep it within m_gridDimensions
        int maxPossibleOffset = m_gridDimensions.x * m_gridDimensions.y - 1;
        int offsetWithinBounds = std::min<int>(std::max(0, cellIndexOfQuery + offset), maxPossibleOffset);

        for(auto boid : m_grid[offsetWithinBounds]) {
            if(boid == query) continue; // ignore itself

            if(getSquaredDistance(boid->getPosition(),query->getPosition()) <= radius * radius) {
                res.push_back(boid);
            }
        }
    }

    return res;
}

void SpatialHashGrid::removeBoid(std::shared_ptr<Boid> boid) {
    m_grid[boid->p_spatialIndex].erase(boid); // O(1)
}

void SpatialHashGrid::updateBoid(std::shared_ptr<Boid> boid) {
    auto i1 = getIndices(boid->getPosition());
    int key = createKey(i1.x, i1.y);

    if(key == boid->p_spatialIndex) {
        return; // same index occupied, don't update
    }

    removeBoid(boid);
    addBoid(boid);
}

SpatialHashGrid::~SpatialHashGrid() {
    delete[] m_nOffsetsWithinSqDist;
    delete[] m_globalOffset;
}

SpatialHashGrid::SpatialHashGrid(SpatialHashGrid &&other) noexcept {
    m_cellSize = other.m_cellSize;
    m_gridDimensions = other.m_gridDimensions;
    m_windowDimensions = other.m_windowDimensions;
    m_grid = other.m_grid;
    m_nOffsetsWithinSqDist = other.m_nOffsetsWithinSqDist;
    m_globalOffset = other.m_globalOffset;

    other.m_nOffsetsWithinSqDist = nullptr;
    other.m_globalOffset = nullptr;
}

SpatialHashGrid &SpatialHashGrid::operator=(SpatialHashGrid &&other) noexcept {
    if(this != &other) {
        delete[] m_nOffsetsWithinSqDist;
        delete[] m_globalOffset;

        m_cellSize = other.m_cellSize;
        m_gridDimensions = other.m_gridDimensions;
        m_windowDimensions = other.m_windowDimensions;
        m_grid = other.m_grid;
        m_nOffsetsWithinSqDist = other.m_nOffsetsWithinSqDist;
        m_globalOffset = other.m_globalOffset;

        other.m_nOffsetsWithinSqDist = nullptr;
        other.m_globalOffset = nullptr;
    }
    return *this;
}

