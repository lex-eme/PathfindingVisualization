#pragma once
#include <queue>

#include "../WorldMap.h"
#include "../Node.h"
#include "../data structures/NodeQueue.h"
#include "PF.h"


class PF_BFS final : public PF {
    int m_goalX = -1;
    int m_goalY = -1;

    std::vector<Node> m_nodes;
    Node* m_node = nullptr;
    NodeQueue m_openList;
    std::vector<Node*> m_closedList;

public:
    explicit PF_BFS(WorldMap& map);
    ~PF_BFS() override;

    void startSearch(int sx, int sy, int gx, int gy) override;
    void searchIteration() override;

    [[nodiscard]] std::vector<Action> getClosedList() const override;
    [[nodiscard]] std::vector<Action> getOpenList() const override;
    [[nodiscard]] std::vector<Action> getPath() const override;

private:
    void expand();
    [[nodiscard]] bool isInClosedList(int x, int y) const;
    [[nodiscard]] bool isInOpenList(int x, int y) const;
};
