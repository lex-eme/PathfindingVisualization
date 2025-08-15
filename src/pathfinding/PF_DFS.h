#pragma once
#include <queue>

#include "../Node.h"
#include "PF.h"


class PF_DFS final : public PF {
    int m_goalX = -1;
    int m_goalY = -1;

    Node* m_node = nullptr;
    std::vector<Node*> m_openList;
    std::vector<Node*> m_closedList;

public:
    explicit PF_DFS(WorldMap& map);
    ~PF_DFS() override;

    void startSearch(int sx, int sy, int gx, int gy) override;
    void searchIteration() override;

    [[nodiscard]] std::vector<Action> getClosedList() const override;
    [[nodiscard]] std::vector<Action> getOpenList() const override;
    [[nodiscard]] std::vector<Action> getPath() const override;

private:
    void expand();
    [[nodiscard]] bool isInClosedList(int x, int y) const;
    [[nodiscard]] bool isInOpenList(int x, int y) const;
    void freeRemainingOpenList();
    void freeRemainingClosedList();
};
