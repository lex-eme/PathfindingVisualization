#pragma once
#include "PF.h"

class PF_BeFS;

class NodePriorityQueue {
    friend PF_BeFS;

    Node** m_nodes;
    size_t m_length = 64;
    size_t m_count = 0;

public:
    NodePriorityQueue();
    ~NodePriorityQueue();

    [[nodiscard]] bool empty() const;

    void push(Node* node);
    Node* pop();
    void clear();

private:
    [[nodiscard]] static size_t left(size_t index);
    [[nodiscard]] static size_t right(size_t index);
    [[nodiscard]] static size_t parent(size_t index);
};

class PF_BeFS final : public PF {
    int m_goalX = -1;
    int m_goalY = -1;

    Node* m_node = nullptr;
    NodePriorityQueue m_openList;
    std::vector<Node*> m_closedList;

public:
    explicit PF_BeFS(WorldMap& map);
    ~PF_BeFS() override;

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
