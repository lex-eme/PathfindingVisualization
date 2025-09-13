#include "Benchmark.h"
#include "PathFindingGUI.h"


int main() {
    WorldMap map;
    PF* pfs[] = {
        PF::Create(PF::BFS, map),
        PF::Create(PF::DFS, map),
        PF::Create(PF::BeFS, map),
        PF::Create(PF::AStar, map)
    };

    PathFindingGUI gui(map, pfs);
    gui.run();
    // const Benchmark benchmark(pfs);
    // benchmark.run(PF::COUNT);

    delete pfs[0];
    delete pfs[1];
    delete pfs[2];
    delete pfs[3];

    return 0;
}
