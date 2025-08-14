#include "PathFindingGUI.h"

#include <imgui-SFML.h>
#include <imgui.h>

PathFindingGUI::PathFindingGUI(WorldMap& map) : m_map(map), m_bfs(m_map),
                                                m_configMenu(200.0f, m_map.getBounds().y * m_tileSize,
                                                             m_map.getBounds().x * m_tileSize, 0.0f, this, m_config) {
    initWindow();
    m_bfs.startSearch(m_startPosition.x, m_startPosition.y, m_goalPosition.x, m_goalPosition.y);
}

void PathFindingGUI::run() {
    sf::Clock deltaClock;
    while (m_running) {
        const auto deltaTime = deltaClock.restart();
        ImGui::SFML::Update(m_window, deltaTime);
        sUserInput();

        ImGui::ShowDemoWindow();

        update(deltaTime);
        sRender();

        ImGui::SFML::Render(m_window);
        m_window.display();
    }
    ImGui::SFML::Shutdown();
    m_window.close();
}

void PathFindingGUI::initWindow() {
    const auto bounds = m_map.getBounds();
    m_window.create(sf::VideoMode(sf::Vector2u(bounds.x * m_tileSize + 200, bounds.y * m_tileSize)), "Pathfinding",
                    sf::Style::Titlebar | sf::Style::Close);
    m_window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(m_window);
}

void PathFindingGUI::loadMap(const std::string& path) const {
    m_map.loadFromFile(path);
}

void PathFindingGUI::sUserInput() {
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        ImGui::SFML::ProcessEvent(m_window, event.value());

        if (event->is<sf::Event::Closed>()) {
            m_running = false;
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Escape:
                    m_running = false;
                    break;
                case sf::Keyboard::Key::N:
                    m_bfs.searchIteration();
                    break;
                default: break;
            }
        } else if (const auto* buttonClicked = event->getIf<sf::Event::MouseButtonPressed>()) {
            switch (buttonClicked->button) {
                case sf::Mouse::Button::Left: {
                    if (isInViewport(buttonClicked->position)) {
                        m_startPosition = screenToWorld(buttonClicked->position);
                        restart();
                    }
                    break;
                }
                case sf::Mouse::Button::Right: {
                    if (isInViewport(buttonClicked->position)) {
                        m_goalPosition = screenToWorld(buttonClicked->position);
                        restart();
                    }
                }
                default: break;
            }
        }
    }
}

void PathFindingGUI::update(const sf::Time deltaTime) {
    if (m_config.typeIndex == 1) {
        m_timeSinceLastAnimation += deltaTime.asSeconds();
        if (m_timeSinceLastAnimation >= m_animationTime * m_config.animMultiplier) {
            m_bfs.searchIteration();
            m_timeSinceLastAnimation = 0.0f;
        }
    }

    m_configMenu.sRender();
}

void PathFindingGUI::sRender() {
    m_window.clear();

    sf::RectangleShape rect;
    rect.setSize({m_tileSize, m_tileSize});

    const auto bounds = m_map.getBounds();
    for (int y = 0; y < bounds.y; y++) {
        for (int x = 0; x < bounds.x; x++) {
            const int val = m_map.getTile(x, y);

            if (val == 0) {
                rect.setFillColor(sf::Color::Blue);
            } else if (val == 1) {
                rect.setFillColor(sf::Color(100, 100, 100));
            } else if (val == -1) {
                rect.setFillColor(sf::Color::Yellow);
            } else if (val == -2) {
                rect.setFillColor(sf::Color::Cyan);
            } else {
                rect.setFillColor(sf::Color::Black);
            }

            rect.setPosition({x * m_tileSize, y * m_tileSize});
            m_window.draw(rect);
        }
    }

    rect.setFillColor(sf::Color::Red);

    for (const auto [x, y]: m_bfs.getClosedList()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color(255, 128, 0));

    for (const auto [x, y]: m_bfs.getOpenList()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color::White);

    for (const auto [x, y]: m_bfs.getPath()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color::Yellow);
    rect.setPosition({m_bfs.getStart().x * m_tileSize, m_bfs.getStart().y * m_tileSize});
    m_window.draw(rect);

    rect.setFillColor(sf::Color::Cyan);
    rect.setPosition({m_bfs.getGoal().x * m_tileSize, m_bfs.getGoal().y * m_tileSize});
    m_window.draw(rect);

    if (m_config.showGrid) {
        for (int i = 0; i < bounds.x; i++) {
            drawLine(i * m_tileSize, 0, i * m_tileSize, bounds.y * m_tileSize, sf::Color(30, 30, 30));
        }

        for (int j = 0; j < bounds.y; j++) {
            drawLine(0, j * m_tileSize, bounds.x * m_tileSize, j * m_tileSize, sf::Color(30, 30, 30));
        }
    }
}

void PathFindingGUI::drawLine(float x1, float y1, float x2, float y2, const sf::Color color) {
    const sf::Vertex line[] = {
        {{x1, y1}, color, {0.0f, 0.0f}},
        {{x2, y2}, color, {0.0f, 0.0f}}
    };

    m_window.draw(line, 2, sf::PrimitiveType::LineStrip);
}

void PathFindingGUI::restart() {
    if (m_config.typeIndex == 0) {
        m_bfs.solve(m_startPosition.x, m_startPosition.y, m_goalPosition.x, m_goalPosition.y);
    } else if (m_config.typeIndex <= 3) {
        m_bfs.startSearch(m_startPosition.x, m_startPosition.y, m_goalPosition.x, m_goalPosition.y);
    }
}

sf::Vector2i PathFindingGUI::screenToWorld(const sf::Vector2i screenPos) const {
    const int size = static_cast<int>(m_tileSize);
    return screenPos / size;
}

bool PathFindingGUI::isInViewport(const sf::Vector2i screenPos) const {
    return screenPos.x < (m_map.getBounds().x * m_tileSize);
}
