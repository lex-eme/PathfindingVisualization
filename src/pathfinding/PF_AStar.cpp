#include "PF_AStar.h"


PF_AStar::PF_AStar(WorldMap& map)
    : PF(map, new PriorityQueueOpenList) {
}

int PF_AStar::heuristic(const int x, const int y) const {
    return abs(m_goalX - x) + abs(m_goalY - y) + m_node->m_depth;
}

bool PF_AStar::shouldAddToOpenList(const int x, const int y) const {
    return !isInClosedList(x, y);
}
