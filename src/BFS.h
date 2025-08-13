#pragma once
#include <queue>

#include "WorldMap.h"
#include "Node.h"

class BFS;

class NodeQueue {
    friend BFS;

    Node** m_nodes;
    size_t m_length = 64;
    size_t m_start = 0;
    size_t m_end = 0;
    size_t m_count = 0;

public:
    NodeQueue();

    [[nodiscard]] bool empty() const;

    void push(Node* node);
    Node* pop();
    void clear();
};

class BFS {
    WorldMap& m_map;
    int m_startX = -1;
    int m_startY = -1;
    int m_goalX = -1;
    int m_goalY = -1;
    int m_cost = 0;

    bool m_inProgress = false;
    Node* m_node = nullptr;
    NodeQueue m_openList;
    std::vector<Node*> m_closedList;

public:
    explicit BFS(WorldMap& map);
    ~BFS();

    void solve(int sx, int sy, int gx, int gy);

    void startSearch(int sx, int sy, int gx, int gy);
    void searchIteration();

    [[nodiscard]] Action getStart() const;
    [[nodiscard]] Action getGoal() const;
    [[nodiscard]] std::vector<Action> getClosedList() const;
    [[nodiscard]] std::vector<Action> getOpenList() const;
    [[nodiscard]] std::vector<Action> getPath() const;

private:
    void expand();
    bool isInClosedList(const Node* node) const;
    bool isInClosedList(int x, int y) const;
    bool isInOpenList(int x, int y) const;
    void freeRemainingOpenList();
    void freeRemainingClosedList();
};
