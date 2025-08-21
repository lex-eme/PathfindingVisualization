#include "Benchmark.h"

#include <fstream>

#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"


Benchmark::Benchmark(PF** pfs): m_pfs(pfs) {
}

void Benchmark::run(const int algorithmCount) const {
    int goals[5][4] = {
        {4, 15, 18, 0},
        {13, 15, 31, 3},
        {13, 15, 4, 15},
        {15, 0, 16, 14},
        {7, 5, 27, 13}
    };


    sf::Clock clock;
    std::ofstream fos("./file.txt");

    for (size_t pfIndex = 0; pfIndex < algorithmCount; pfIndex++) {
        PF* pf = m_pfs[pfIndex];

        clock.restart();
        for (size_t iteration = 0; iteration < 100; iteration++) {
            for (const auto& goalIndex: goals) {
                pf->solve(goalIndex[0], goalIndex[1], goalIndex[2], goalIndex[3]);
            }
        }

        fos << clock.getElapsedTime().asMicroseconds() << "\n";
    }

    fos.close();
}
