#pragma once
#include "../WorldMap.h"
#include "../OpenList.h"


class PF {
public:
    enum State {
        InProgress, Found, NotFound
    };

    struct Info {
        int pathLength = 0;
        size_t openListSize = 0;
        size_t closedList = 0;
        State state = NotFound;
    };

protected:
    int m_goalX = -1;
    int m_goalY = -1;

    Node* m_node = nullptr;
    std::vector<Node> m_nodes;
    std::vector<Node*> m_closedList;
    OpenList* m_openList;

    const WorldMap& m_map;
    State m_state = NotFound;

public:
    enum Algorithm {
        BFS, DFS, BeFS, AStar, COUNT
    };

    static PF* Create(Algorithm algorithm, WorldMap& map);
    static const char* names[COUNT];

    virtual ~PF();

    void solve(int sx, int sy, int gx, int gy);
    void startSearch(int sx, int sy, int gx, int gy);
    void searchIteration();

    [[nodiscard]] std::vector<Action> getClosedList() const;
    [[nodiscard]] std::vector<Action> getOpenList() const;
    [[nodiscard]] std::vector<Action> getPath() const;
    [[nodiscard]] Info getInfo() const;

protected:
    explicit PF(WorldMap& map, OpenList* openList);

    [[nodiscard]] virtual int heuristic(int x, int y) const;
    [[nodiscard]] virtual bool shouldAddToOpenList(int x, int y) const;
    [[nodiscard]] bool isInClosedList(int x, int y) const;

private:
    void expand();
};
