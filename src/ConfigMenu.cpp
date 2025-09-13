#include "ConfigMenu.h"

#include <imgui.h>

#include "PathFindingGUI.h"

ConfigMenu::ConfigMenu(PathFindingGUI* gui, Config& config)
    : m_width(0), m_height(0), m_x(0), m_y(0), m_gui(gui), m_config(config) {
    readMapsDirectory();
}

void ConfigMenu::render() {
    ImGui::SetNextWindowSize({m_width, m_height});
    ImGui::SetNextWindowPos({m_x, m_y});
    if (ImGui::Begin("Configuration", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("Configuration");
        visualizationSubMenu();
        mapSubMenu();
        algorithmSubMenu();
    }
    ImGui::End();
}

void ConfigMenu::setPosition(const float x, const float y) {
    m_x = x;
    m_y = y;
}

void ConfigMenu::setSize(const float width, const float height) {
    m_width = width;
    m_height = height;
}

float ConfigMenu::getWidth() const {
    return m_width;
}

void ConfigMenu::readMapsDirectory() {
    for (auto const& entry: std::filesystem::directory_iterator("../../assets/maps")) {
        if (is_regular_file(entry)) {
            m_mapNames.push_back(entry.path().filename());
        }
    }

    if (!m_mapNames.empty()) {
        m_gui->setMap(m_mapNames[0]);
    }
}

void ConfigMenu::visualizationSubMenu() const {
    ImGui::SeparatorText("Visualization");

    const char* items[] = {"Instantaneous", "Animated", "Step by step"};

    if (ImGui::BeginCombo("Type", items[m_config.type], ImGuiComboFlags_HeightSmall)) {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
            const bool isSelected = (m_config.type == n);
            if (ImGui::Selectable(items[n], isSelected)) {
                if (n == 0) {
                    m_config.type = Instant;
                } else if (n == 1) {
                    m_config.type = Animated;
                } else {
                    m_config.type = Step;
                }
                m_gui->restart();
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    switch (m_config.type) {
        case Instant: {
            break;
        }
        case Animated: {
            ImGui::SliderFloat("Speed", &m_config.animMultiplier, 2.0f, 0.0f, "%.3f");
            ImGui::SetNextItemWidth(m_width);
            if (ImGui::Button("Restart")) {
                m_gui->restart();
            }
            ImGui::SameLine();
            if (ImGui::Button(m_config.animPaused ? "Play" : "Pause")) {
                m_config.animPaused = !m_config.animPaused;
            }
            ImGui::SameLine();
            if (ImGui::Button("Next")) {
                m_gui->iterate();
            }
            break;
        }
        case Step: {
            if (ImGui::Button("Restart")) {
                m_gui->restart();
            }
            ImGui::SameLine();
            if (ImGui::Button("Next")) {
                m_gui->iterate();
            }
            break;
        }
    }
}

void ConfigMenu::mapSubMenu() const {
    ImGui::SeparatorText("Map");
    static int selectedItem = 0;

    if (ImGui::BeginListBox("##Maps", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing()))) {
        for (int n = 0; n < m_mapNames.size(); n++) {
            const bool isSelected = (selectedItem == n);
            if (ImGui::Selectable(m_mapNames[n].c_str(), isSelected)) {
                selectedItem = n;
                m_gui->setMap(m_mapNames[n]);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
}

void ConfigMenu::algorithmSubMenu() const {
    ImGui::SeparatorText("Algorithm");

    if (ImGui::BeginCombo("Algo", PF::names[m_config.pfIndex], ImGuiComboFlags_HeightSmall)) {
        for (int n = 0; n < IM_ARRAYSIZE(PF::names); n++) {
            const bool is_selected = (m_config.pfIndex == n);
            if (ImGui::Selectable(PF::names[n], is_selected)) {
                m_config.pfIndex = n;
                m_gui->restart();
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    const auto [pathLength, openListSize, closedList, state] = m_gui->getPathInfo();

    switch (state) {
        case PF::InProgress:
            ImGui::Text("State: in progress");
            break;
        case PF::Found:
            ImGui::Text("State: path found");
            break;
        case PF::NotFound:
            ImGui::Text("State: path not found");
            break;
    }

    ImGui::Text("Path length: %d", pathLength);
    ImGui::Text("Open list size: %zu", openListSize);
    ImGui::Text("Closed list size: %zu", closedList);
}
