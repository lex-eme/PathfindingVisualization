#include "WorldMap.h"

#include <fstream>

void WorldMap::loadFromFile(const std::string& path) {
    std::ifstream file(path);

    int val;
    file >> m_width >> m_height;

    m_grid = std::vector<int>(m_width * m_height);

    for (int x = 0; x < m_width * m_height; x++) {
        file >> val;
        m_grid[x] = val;
    }
}

sf::Vector2u WorldMap::getBounds() const {
    return {static_cast<unsigned>(m_width), static_cast<unsigned>(m_height)};
}

int WorldMap::getTile(const int x, const int y) const {
    return m_grid[y * m_width + x];
}

bool WorldMap::isInBounds(const int x, const int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

bool WorldMap::isLegalAction(const int x, const int y, const Action action) const {
    const int currentPosition = getTile(x, y);
    const int newX = x + action.x;
    const int newY = y + action.y;

    if (!isInBounds(newX, newY)) {
        return false;
    }

    const int newPosition = getTile(newX, newY);
    return currentPosition == newPosition;
}
