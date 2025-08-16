#include <iostream>

#include "PathFindingGUI.h"
#include "pathfinding/PF_AStar.h"
#include "pathfinding/PF_BFS.h"
#include "pathfinding/PF.h"
#include "pathfinding/PF_BeFS.h"
#include "pathfinding/PF_DFS.h"


int main() {
    WorldMap map;
    map.loadFromFile("../../assets/map2.txt");

    sf::Clock clock; {
        for (size_t i = 0;  i < 1000; i++) {
            PF_AStar astar(map);
            astar.solve(4, 15, 18, 0);
        }
        std::cout << "A*: " << clock.getElapsedTime().asMicroseconds() << "µs" << std::endl;
    }

    clock.restart();
    {
        for (size_t i = 0;  i < 1000; i++) {
            PF_BFS bfs(map);
            bfs.solve(4, 15, 18, 0);
        }
        std::cout << "bfs: " << clock.getElapsedTime().asMicroseconds() << "µs" << std::endl;
    }

    clock.restart();
    {
        for (size_t i = 0;  i < 1000; i++) {
            PF_BeFS befs(map);
            befs.solve(4, 15, 18, 0);
        }
        std::cout << "befs: " << clock.getElapsedTime().asMicroseconds() << "µs" << std::endl;
    }

    clock.restart();
    {
        for (size_t i = 0;  i < 1000; i++) {
            PF_DFS dfs(map);
            dfs.solve(4, 15, 18, 0);
        }
        std::cout << "dfs: " << clock.getElapsedTime().asMicroseconds() << "µs" << std::endl;
    }

    // PathFindingGUI gui(map);
    // gui.run();
}
