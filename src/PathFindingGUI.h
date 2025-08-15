#pragma once
#include <SFML/Graphics.hpp>

#include "ConfigMenu.h"
#include "pathfinding/PF.h"
#include "WorldMap.h"


class PathFindingGUI {
    sf::RenderWindow m_window;
    sf::Vector2i m_startPosition{-1, -1};
    sf::Vector2i m_goalPosition{-1, -1};
    float m_tileSize = 32.0f;
    bool m_running = true;
    WorldMap& m_map;
    PF* m_pathfinding;

    Config m_config{0, 1.0f, true};
    ConfigMenu m_configMenu;

    float m_animationTime = 0.5f;
    float m_timeSinceLastAnimation = 0.0f;

public:
    explicit PathFindingGUI(WorldMap& map);

    void run();
    void restart() const;

private:
    void initWindow();
    void loadMap(const std::string& path) const;

    void sUserInput();
    void update(sf::Time deltaTime);
    void sRender();
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color);


    sf::Vector2i screenToWorld(sf::Vector2i screenPos) const;
    [[nodiscard]] bool isInViewport(sf::Vector2i screenPos) const;
};
