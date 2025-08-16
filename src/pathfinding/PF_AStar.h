#pragma once
#include "PF.h"
#include "../data structures/NodePriorityQueue.h"


class PF_AStar final : public PF {
    int m_goalX = -1;
    int m_goalY = -1;

    Node* m_node = nullptr;
    NodePriorityQueue m_openList;
    std::vector<Node*> m_closedList;

public:
    explicit PF_AStar(WorldMap& map);
    ~PF_AStar() override;

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
