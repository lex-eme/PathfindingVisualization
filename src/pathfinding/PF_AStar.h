#pragma once
#include "PF.h"

class PF_AStar final : public PF {
public:
    explicit PF_AStar(WorldMap& map);

private:
    [[nodiscard]] int heuristic(int x, int y) const override;
    [[nodiscard]] bool shouldAddToOpenList(int x, int y) const override;
};
