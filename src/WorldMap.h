#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Node.h"


class WorldMap {
    std::vector<int> m_grid;
    int m_width = 0;
    int m_height = 0;

public:
    void loadFromFile(const std::string& path);

    [[nodiscard]] sf::Vector2u getBounds() const;
    [[nodiscard]] int getTile(int x, int y) const;
    [[nodiscard]] bool isInBounds(int x, int y) const;
    [[nodiscard]] bool isLegalAction(int x, int y, Action action) const;
};
