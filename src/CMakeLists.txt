add_executable(main main.cpp
        Node.cpp
        Node.h
        WorldMap.cpp
        WorldMap.h
        PF_BFS.cpp
        PF_BFS.h
        PathFindingGUI.cpp
        PathFindingGUI.h
        ConfigMenu.cpp
        ConfigMenu.h
        PF.cpp
        PF.h
)
target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)