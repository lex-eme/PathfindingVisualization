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
    PF** m_pfs;

    Config m_config{Instant, 1.0f, 0, false, true};
    ConfigMenu m_configMenu;

    float m_animationTime = 0.5f;
    float m_timeSinceLastAnimation = 0.0f;

public:
    PathFindingGUI(WorldMap& map, PF* pfs[]);

    void run();
    void restart() const;
    void iterate() const;

private:
    void initWindow();
    void loadMap(const std::string& path) const;

    void sUserInput();
    void update(sf::Time deltaTime);
    void sRender();
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color);

    void resize(sf::Vector2u size);

    sf::Vector2i screenToWorld(sf::Vector2i screenPos) const;
    [[nodiscard]] bool isInViewport(sf::Vector2i screenPos) const;
};
