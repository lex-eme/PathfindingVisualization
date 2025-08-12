#pragma once
#include <queue>

#include "WorldMap.h"
#include "Node.h"

class BFS {
    WorldMap& m_map;
    int m_startX = -1;
    int m_startY = -1;
    int m_goalX = -1;
    int m_goalY = -1;
    int m_cost = 0;

    bool m_inProgress = false;
    Node* m_node = nullptr;
    std::queue<Node*> m_openList;
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
    [[nodiscard]] std::vector<Action> getPath() const;

private:
    void expand();
    bool isInClosedList(const Node* node) const;
    bool isInClosedList(int x, int y) const;
    void freeRemainingOpenList();
    void freeRemainingClosedList();
};
