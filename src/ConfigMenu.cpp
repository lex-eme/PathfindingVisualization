#include "ConfigMenu.h"

#include <imgui.h>

#include "PathFindingGUI.h"

ConfigMenu::ConfigMenu(const float m_width, const float m_height, const float m_x, const float m_y, PathFindingGUI* gui,
                       Config& m_config)
    : m_width(m_width),
      m_height(m_height),
      m_x(m_x),
      m_y(m_y),
      gui(gui),
      m_config(m_config) {
}

void ConfigMenu::sRender() {
    ImGui::SetNextWindowSize({m_width, m_height});
    ImGui::SetNextWindowPos({m_x, m_y});
    if (ImGui::Begin("Configuration", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("Configuration");
        ImGui::Checkbox("Show grid", &m_config.showGrid);

        const char* items[] = {"Instantaneous", "Animated", "Step by step"};

        if (ImGui::BeginCombo("combo 1", items[m_config.typeIndex], ImGuiComboFlags_HeightSmall)) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                const bool is_selected = (m_config.typeIndex == n);
                if (ImGui::Selectable(items[n], is_selected)) {
                    m_config.typeIndex = n;
                    gui->restart();
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Restart")) {
            gui->restart();
        }
    }
    ImGui::End();
}
