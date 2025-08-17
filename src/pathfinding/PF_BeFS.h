#pragma once
#include "PF.h"


class PF_BeFS final : public PF {
public:
    explicit PF_BeFS(WorldMap& map);

private:
    [[nodiscard]] int heuristic(int x, int y) const override;
};
