#pragma once

class PathFindingGUI;

struct Config {
    int typeIndex;
    float animMultiplier;
    bool showGrid;
};

class ConfigMenu {
    float m_width;
    float m_height;
    float m_x;
    float m_y;

    PathFindingGUI* gui;
    Config& m_config;

public:
    ConfigMenu(float m_width, float m_height, float m_x, float m_y, PathFindingGUI* gui, Config& m_config);

    void sRender();
};
