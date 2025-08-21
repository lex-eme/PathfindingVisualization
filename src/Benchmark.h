#pragma once
#include "pathfinding/PF.h"


class Benchmark {
    PF** m_pfs;

public:
    explicit Benchmark(PF** pfs);

    void run(int algorithmCount) const;
};
