#include "PathFindingGUI.h"

int main() {
    WorldMap map;
    map.loadFromFile("../../assets/map2.txt");
    PathFindingGUI gui(map);
    gui.run();
}
