add_executable(main main.cpp
        Node.cpp
        Node.h
        WorldMap.cpp
        WorldMap.h
        pathfinding/PF_BFS.cpp
        pathfinding/PF_BFS.h
        PathFindingGUI.cpp
        PathFindingGUI.h
        ConfigMenu.cpp
        ConfigMenu.h
        pathfinding/PF.cpp
        pathfinding/PF.h
        pathfinding/PF_DFS.cpp
        pathfinding/PF_DFS.h
        pathfinding/PF_BeFS.cpp
        pathfinding/PF_BeFS.h
)
target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)