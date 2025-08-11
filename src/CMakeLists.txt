add_executable(main main.cpp
        Node.cpp
        Node.h
        WorldMap.cpp
        WorldMap.h
        BFS.cpp
        BFS.h
        PathFindingGUI.cpp
        PathFindingGUI.h
)
target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)