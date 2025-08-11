#include "PathFindingGUI.h"

#include <imgui-SFML.h>
#include <imgui.h>


PathFindingGUI::PathFindingGUI() : m_bfs(m_map) {
    loadMap();
    init();
    m_bfs.startSearch(startPosition.x, startPosition.y, goalPosition.x, goalPosition.y);
}

void PathFindingGUI::run() {
    sf::Clock deltaClock;
    while (m_running) {
        sUserInput();
        ImGui::SFML::Update(m_window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        sRender();
    }
    ImGui::SFML::Shutdown();
}

void PathFindingGUI::init() {
    const auto bounds = m_map.getBounds();
    m_window.create(sf::VideoMode(sf::Vector2u(bounds.x * m_tileSize, bounds.y * m_tileSize)), "Pathfinding");
    m_window.setFramerateLimit(60);
    ImGui::SFML::Init(m_window);
}

void PathFindingGUI::loadMap() {
    m_map.loadFromFile("../../assets/map2.txt");
}

void PathFindingGUI::sUserInput() {
    m_window.handleEvents(
        [&]<typename Event>(const Event& event) {
            ImGui::SFML::ProcessEvent(m_window, event);

            if constexpr (std::is_same_v<std::decay_t<Event>, sf::Event::Closed>) {
                m_running = false;
            } else if constexpr (std::is_same_v<std::decay_t<Event>, sf::Event::KeyPressed>) {
                switch (event.code) {
                    case sf::Keyboard::Key::N:
                        m_bfs.searchIteration();
                        break;
                    case sf::Keyboard::Key::Escape:
                        m_running = false;
                        break;
                    default: break;
                }
            } else if constexpr (std::is_same_v<std::decay_t<Event>, sf::Event::MouseButtonPressed>) {
                switch (event.button) {
                    case sf::Mouse::Button::Left: {
                        startPosition = screenToWorld(event.position);
                        m_bfs.solve(startPosition.x, startPosition.y, goalPosition.x, goalPosition.y);
                        //m_bfs.startSearch(startPosition.x, startPosition.y, goalPosition.x, goalPosition.y);
                        break;
                    }
                    case sf::Mouse::Button::Right: {
                        goalPosition = screenToWorld(event.position);
                        m_bfs.solve(startPosition.x, startPosition.y, goalPosition.x, goalPosition.y);
                        //m_bfs.startSearch(startPosition.x, startPosition.y, goalPosition.x, goalPosition.y);
                    }
                    default: break;
                }
            }
        });
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

    for (int i = 0; i < bounds.x; i++) {
        drawLine(i * m_tileSize, 0, i * m_tileSize, bounds.y * m_tileSize, sf::Color(30, 30, 30));
    }

    for (int j = 0; j < bounds.y; j++) {
        drawLine(0, j * m_tileSize, bounds.x * m_tileSize, j * m_tileSize, sf::Color(30, 30, 30));
    }

    ImGui::SFML::Render(m_window);
    m_window.display();
}

void PathFindingGUI::drawLine(float x1, float y1, float x2, float y2, const sf::Color color) {
    const sf::Vertex line[] = {
        {{x1, y1}, color, {0.0f, 0.0f}},
        {{x2, y2}, color, {0.0f, 0.0f}}
    };

    m_window.draw(line, 2, sf::PrimitiveType::LineStrip);
}

sf::Vector2i PathFindingGUI::screenToWorld(const sf::Vector2i screenPos) const {
    const int size = static_cast<int>(m_tileSize);
    return screenPos / size;
}
