#pragma once
#include "../WorldMap.h"
#include "../OpenList.h"


class PF {
protected:
    int m_goalX = -1;
    int m_goalY = -1;

    Node* m_node = nullptr;
    std::vector<Node> m_nodes;
    std::vector<Node*> m_closedList;
    OpenList* m_openList;

    WorldMap& m_map;
    bool m_inProgress = false;

public:
    explicit PF(WorldMap& map, OpenList* openList);
    virtual ~PF();

    void solve(int sx, int sy, int gx, int gy);
    void startSearch(int sx, int sy, int gx, int gy);
    void searchIteration();

    [[nodiscard]] std::vector<Action> getClosedList() const;
    [[nodiscard]] std::vector<Action> getOpenList() const;
    [[nodiscard]] std::vector<Action> getPath() const;

protected:
    [[nodiscard]] virtual int heuristic(int x, int y) const;
    [[nodiscard]] virtual bool shouldAddToOpenList(int x, int y) const;
    [[nodiscard]] bool isInClosedList(int x, int y) const;

private:
    void expand();
};
