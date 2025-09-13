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
    WorldMap& m_map;
    PF** m_pfs;

    Config m_config{Instant, 1.0f, 0, false, true};
    const std::string m_mapsDirectory = "../../assets/maps/";
    ConfigMenu m_configMenu;

    float m_animationTime = 0.5f;
    float m_timeSinceLastAnimation = 0.0f;

    sf::Vector2i m_oldMousePosition;
    bool m_isPanning = false;
    bool m_running = true;

public:
    PathFindingGUI(WorldMap& map, PF* pfs[]);

    void run();
    void restart() const;
    void iterate() const;
    void setMap(const std::string& name);
    PF::Info getPathInfo() const;

private:
    void initWindow();
    void loadMap(const std::string& path) const;

    void userInput();
    void windowResized(sf::Vector2u size);
    void keyPressed(const sf::Event::KeyPressed* keyPressed);
    void mouseButtonPressed(const sf::Event::MouseButtonPressed* buttonClicked);
    void mouseButtonReleased(const sf::Event::MouseButtonReleased* buttonReleased);
    void mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll);
    void mouseMoved(const sf::Event::MouseMoved* move);

    void update(sf::Time deltaTime);
    void render();
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color);


    sf::Vector2i screenToWorld(sf::Vector2i screenPos) const;
    [[nodiscard]] bool isInViewport(sf::Vector2i screenPos) const;
};
