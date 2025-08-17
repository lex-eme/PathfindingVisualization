#include "PF_BeFS.h"


PF_BeFS::PF_BeFS(WorldMap& map)
    : PF(map, new PriorityQueueOpenList) {
}

int PF_BeFS::heuristic(const int x, const int y) const {
    return abs(m_goalX - x) + abs(m_goalY - y);
}