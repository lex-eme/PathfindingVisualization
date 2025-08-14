#pragma once
#include "WorldMap.h"


class PF {
protected:
    WorldMap& m_map;
    bool m_inProgress = false;

public:
    explicit PF(WorldMap& map);
    virtual ~PF() = default;

    void solve(int sx, int sy, int gx, int gy);
    virtual void startSearch(int sx, int sy, int gx, int gy) = 0;
    virtual void searchIteration() = 0;

    [[nodiscard]] virtual std::vector<Action> getClosedList() const = 0;
    [[nodiscard]] virtual std::vector<Action> getOpenList() const = 0;
    [[nodiscard]] virtual std::vector<Action> getPath() const = 0;
};
