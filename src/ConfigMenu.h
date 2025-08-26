#pragma once
#include <cstddef>

class PathFindingGUI;

enum VisualizationType: int {
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

    PathFindingGUI* m_gui;
    Config& m_config;

public:
    ConfigMenu(float m_width, float m_height, float m_x, float m_y, PathFindingGUI* gui, Config& m_config);

    void render();
    void setPosition(float x, float y);
    void setSize(float width, float height);
    [[nodiscard]] float getWidth() const;

private:
    void visualizationSubMenu() const;
    void mapSubMenu() const;
    void algorithmSubMenu() const;
};
