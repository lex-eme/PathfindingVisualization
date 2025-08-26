#include "PathFindingGUI.h"

#include <imgui-SFML.h>
#include <imgui.h>


constexpr float ConfigMenuWidth = 250.0f;

PathFindingGUI::PathFindingGUI(WorldMap& map, PF* pfs[]) : m_map(map),
                                                           m_pfs(pfs),
                                                           m_configMenu(
                                                               ConfigMenuWidth, m_map.getBounds().y * m_tileSize,
                                                               m_map.getBounds().x * m_tileSize - ConfigMenuWidth, 0.0f,
                                                               this,
                                                               m_config) {
    initWindow();
    m_pfs[m_config.pfIndex]->startSearch(m_startPosition.x, m_startPosition.y, m_goalPosition.x, m_goalPosition.y);
}

void PathFindingGUI::run() {
    sf::Clock deltaClock;
    while (m_running) {
        const auto deltaTime = deltaClock.restart();
        ImGui::SFML::Update(m_window, deltaTime);
        userInput();

        update(deltaTime);
        render();

        ImGui::SFML::Render(m_window);
        m_window.display();
    }
    ImGui::SFML::Shutdown();
    m_window.close();
}

void PathFindingGUI::initWindow() {
    const auto bounds = m_map.getBounds();
    m_window.create(sf::VideoMode(sf::Vector2u(bounds.x * m_tileSize, bounds.y * m_tileSize)), "Pathfinding");
    m_window.setVerticalSyncEnabled(true);
    windowResized(sf::Vector2u(bounds.x * m_tileSize, bounds.y * m_tileSize));
    ImGui::SFML::Init(m_window, false);
    const ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../../assets/fonts/Roboto-Black.ttf", 12.0f);
    ImGui::SFML::UpdateFontTexture();
    m_oldMousePosition = sf::Mouse::getPosition();
}

void PathFindingGUI::loadMap(const std::string& path) const {
    m_map.loadFromFile(path);
}

void PathFindingGUI::userInput() {
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        ImGui::SFML::ProcessEvent(m_window, event.value());

        if (event->is<sf::Event::Closed>()) {
            m_running = false;
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            windowResized(resized->size);
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            keyPressed(key);
        } else if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            mouseButtonPressed(buttonPressed);
        } else if (const auto* buttonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            mouseButtonReleased(buttonReleased);
        } else if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
            mouseWheelScrolled(scroll);
        } else if (const auto* move = event->getIf<sf::Event::MouseMoved>()) {
            mouseMoved(move);
        }
    }
}

void PathFindingGUI::windowResized(const sf::Vector2u size) {
    const float menuWidth = m_configMenu.getWidth();

    m_configMenu.setPosition(size.x - menuWidth, 0.0f);
    m_configMenu.setSize(menuWidth, size.y);

    sf::View view = m_window.getView();
    float xRatio = (size.x - menuWidth) / size.x;
    view.setViewport(sf::FloatRect({0.0f, 0.0f}, {xRatio, 1.0f}));
    view.setSize({size.x - menuWidth, static_cast<float>(size.y)});
    m_window.setView(view);
}

void PathFindingGUI::keyPressed(const sf::Event::KeyPressed* keyPressed) {
    switch (keyPressed->code) {
        case sf::Keyboard::Key::Escape:
            m_running = false;
            break;
        case sf::Keyboard::Key::N:
            iterate();
            break;
        default: break;
    }
}

void PathFindingGUI::mouseButtonPressed(const sf::Event::MouseButtonPressed* buttonClicked) {
    switch (buttonClicked->button) {
        case sf::Mouse::Button::Left: {
            if (isInViewport(buttonClicked->position)) {
                m_goalPosition = screenToWorld(buttonClicked->position);
                restart();
            }
            break;
        }
        case sf::Mouse::Button::Right: {
            if (isInViewport(buttonClicked->position)) {
                m_startPosition = screenToWorld(buttonClicked->position);
                restart();
            }
            break;
        }
        case sf::Mouse::Button::Middle: {
            m_oldMousePosition = buttonClicked->position;
            m_isPanning = true;
        }
        default: break;
    }
}

void PathFindingGUI::mouseButtonReleased(const sf::Event::MouseButtonReleased* buttonReleased) {
    switch (buttonReleased->button) {
        case sf::Mouse::Button::Middle: {
            m_isPanning = false;
        }
        default: break;
    }
}

void PathFindingGUI::mouseWheelScrolled(const sf::Event::MouseWheelScrolled* scroll) {
    auto view = m_window.getView();
    if (scroll->delta < 0.0f) {
        view.zoom(1.0f / 0.9f);
    } else if (scroll->delta > 0.0f) {
        view.zoom(0.9f);
    }
    m_window.setView(view);
}

void PathFindingGUI::mouseMoved(const sf::Event::MouseMoved* move) {
    if (m_isPanning) {
        auto view = m_window.getView();
        auto deltaPosition = m_window.mapPixelToCoords(m_oldMousePosition) - m_window.
                             mapPixelToCoords(move->position);
        view.move(sf::Vector2f(deltaPosition));
        m_window.setView(view);
        m_oldMousePosition = move->position;
    }
}

void PathFindingGUI::update(const sf::Time deltaTime) {
    if (m_config.type == Animated && !m_config.animPaused) {
        m_timeSinceLastAnimation += deltaTime.asSeconds();
        if (m_timeSinceLastAnimation >= m_animationTime * m_config.animMultiplier) {
            m_pfs[m_config.pfIndex]->searchIteration();
            m_timeSinceLastAnimation = 0.0f;
        }
    }

    m_configMenu.render();
}

void PathFindingGUI::render() {
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

    for (const auto [x, y]: m_pfs[m_config.pfIndex]->getClosedList()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color(255, 128, 0));

    for (const auto [x, y]: m_pfs[m_config.pfIndex]->getOpenList()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color::White);

    for (const auto [x, y]: m_pfs[m_config.pfIndex]->getPath()) {
        rect.setPosition({x * m_tileSize, y * m_tileSize});
        m_window.draw(rect);
    }

    rect.setFillColor(sf::Color::Yellow);
    rect.setPosition({m_startPosition.x * m_tileSize, m_startPosition.y * m_tileSize});
    m_window.draw(rect);

    rect.setFillColor(sf::Color::Cyan);
    rect.setPosition({m_goalPosition.x * m_tileSize, m_goalPosition.y * m_tileSize});
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

void PathFindingGUI::restart() const {
    switch (m_config.type) {
        case Instant:
            m_pfs[m_config.pfIndex]->solve(m_startPosition.x, m_startPosition.y, m_goalPosition.x, m_goalPosition.y);
            break;
        case Animated:
        case Step:
            m_pfs[m_config.pfIndex]->startSearch(m_startPosition.x, m_startPosition.y, m_goalPosition.x,
                                                 m_goalPosition.y);
            break;
    }
}

void PathFindingGUI::iterate() const {
    m_pfs[m_config.pfIndex]->searchIteration();
}

PF::Info PathFindingGUI::getPathInfo() const {
    return m_pfs[m_config.pfIndex]->getInfo();
}

sf::Vector2i PathFindingGUI::screenToWorld(const sf::Vector2i screenPos) const {
    const int size = static_cast<int>(m_tileSize);
    return sf::Vector2i(m_window.mapPixelToCoords(screenPos)) / size;
}

bool PathFindingGUI::isInViewport(const sf::Vector2i screenPos) const {
    return screenPos.x < m_window.getSize().x - m_configMenu.getWidth();
}
