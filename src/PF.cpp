#include "PF.h"

PF::PF(WorldMap& map) : m_map(map) {
}

void PF::solve(const int sx, const int sy, const int gx, const int gy) {
    startSearch(sx, sy, gx, gy);
    while (m_inProgress) {
        searchIteration();
    }
}
