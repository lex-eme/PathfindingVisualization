#include "PF.h"

#include <algorithm>

#include "PF_AStar.h"
#include "PF_BeFS.h"
#include "PF_BFS.h"
#include "PF_DFS.h"

PF* PF::Create(const Algorithm algorithm, WorldMap& map) {
    switch (algorithm) {
        case BFS: return new PF_BFS(map);
        case DFS: return new PF_DFS(map);
        case BeFS: return new PF_BeFS(map);
        case AStar: return new PF_AStar(map);
        default: return nullptr;
    }
}

const char* PF::names[] = {"Breadth first search", "Depth first search", "Best first search", "A*"};

PF::PF(WorldMap& map, OpenList* openList): m_openList(openList), m_map(map) {
    m_nodes.reserve(1024);
}

PF::~PF() {
    delete m_openList;
}

void PF::solve(const int sx, const int sy, const int gx, const int gy) {
    startSearch(sx, sy, gx, gy);
    while (m_state == InProgress) {
        searchIteration();
    }
}

void PF::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_goalX = gx;
    m_goalY = gy;

    m_state = InProgress;
    m_openList->clear();
    m_closedList.clear();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList->push(m_node);
}

void PF::searchIteration() {
    if (m_state != InProgress) {
        return;
    }

    if (m_openList->empty()) {
        m_state = NotFound;
        return;
    }

    m_node = m_openList->pop();

    if (isInClosedList(m_node->m_x, m_node->m_y)) {
        return;
    }

    if (m_node->m_x == m_goalX && m_node->m_y == m_goalY) {
        m_state = Found;
        return;
    }

    m_closedList.push_back(m_node);

    expand();
}

std::vector<Action> PF::getClosedList() const {
    std::vector<Action> result;
    result.reserve(m_closedList.size());

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF::getOpenList() const {
    return m_openList->getStates();
}

std::vector<Action> PF::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

PF::Info PF::getInfo() const {
    return {m_node->m_depth, m_openList->size(), m_closedList.size(), m_state};
}

void PF::expand() {
    // ACTIONS                                          UP                DOWN              LEFT             RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (auto action: actions) {
        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;

            if (shouldAddToOpenList(newX, newY)) {
                m_nodes.emplace_back(m_node, action, newX, newY, m_node->m_depth + 1, heuristic(newX, newY));
                m_openList->push(&m_nodes.back());
            }
        }
    }
}

int PF::heuristic(int x, int y) const {
    return 0;
}

bool PF::shouldAddToOpenList(const int x, const int y) const {
    return !isInClosedList(x, y) && !m_openList->containsState(x, y);
}

bool PF::isInClosedList(int x, int y) const {
    return std::any_of(m_closedList.begin(), m_closedList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}
