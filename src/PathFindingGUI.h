#pragma once
#include <SFML/Graphics.hpp>

#include "BFS.h"
#include "ConfigMenu.h"
#include "WorldMap.h"


class PathFindingGUI {
    sf::RenderWindow m_window;
    sf::Vector2i startPosition {-1, -1};
    sf::Vector2i goalPosition {-1, -1};
    float m_tileSize = 32.0f;
    bool m_running = true;
    WorldMap& m_map;
    BFS m_bfs;

    Config m_config {0, true};
    ConfigMenu m_configMenu;

public:
    explicit PathFindingGUI(WorldMap& map);

    void run();
    void restart();

private:
    void initWindow();
    void loadMap(const std::string& path) const;

    void sUserInput();
    void sRender();
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color);


    sf::Vector2i screenToWorld(sf::Vector2i screenPos) const;
    [[nodiscard]] bool isInViewport(sf::Vector2i screenPos) const;
};
