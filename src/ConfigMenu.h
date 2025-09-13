#pragma once
#include <cstddef>
#include <vector>

class PathFindingGUI;

enum VisualizationType {
    Instant, Animated, Step
};

struct Config {
    VisualizationType type;
    float animMultiplier;
    size_t pfIndex;
    bool animPaused;
    bool showGrid;
};

class ConfigMenu {
    float m_width;
    float m_height;
    float m_x;
    float m_y;

    std::vector<std::string> m_mapNames;

    PathFindingGUI* m_gui;
    Config& m_config;

public:
    ConfigMenu(PathFindingGUI* gui, Config& config);

    void render();
    void setPosition(float x, float y);
    void setSize(float width, float height);
    [[nodiscard]] float getWidth() const;

private:
    void readMapsDirectory();

    void visualizationSubMenu() const;
    void mapSubMenu() const;
    void algorithmSubMenu() const;
};
