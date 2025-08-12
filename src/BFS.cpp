#include "BFS.h"

BFS::BFS(WorldMap& map) : m_map(map) {
}

BFS::~BFS() {
    freeRemainingOpenList();
    freeRemainingClosedList();
}

void BFS::solve(const int sx, const int sy, const int gx, const int gy) {
    startSearch(sx, sy, gx, gy);
    while (m_inProgress) {
        searchIteration();
    }
}

void BFS::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_startX = sx;
    m_startY = sy;
    m_goalX = gx;
    m_goalY = gy;

    m_inProgress = true;
    freeRemainingOpenList();
    freeRemainingClosedList();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList.push(m_node);
}

void BFS::searchIteration() {
    if (!m_inProgress) {
        return;
    }

    if (m_openList.empty()) {
        m_inProgress = false;
        return;
    }

    m_node = m_openList.front();
    m_openList.pop();

    if (isInClosedList(m_node)) {
        return;
    }

    if (m_node->m_x == m_goalX && m_node->m_y == m_goalY) {
        m_inProgress = false;
        return;
    }

    m_closedList.push_back(m_node);

    expand();
}

Action BFS::getStart() const {
    return {m_startX, m_startY};
}

Action BFS::getGoal() const {
    return {m_goalX, m_goalY};
}

std::vector<Action> BFS::getClosedList() const {
    std::vector<Action> result;

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> BFS::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

void BFS::expand() {
    // ACTIONS                                  UP            DOWN                  LEFT            RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (int i = 0; i < actions.size(); i++) {
        const Action action = actions[i];

        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;
            if (!isInClosedList(newX, newY)) {
                auto newNode = new Node(m_node, actions[i], newX, newY, m_node->m_depth + 1);
                m_openList.push(newNode);
            }
        }
    }
}

bool BFS::isInClosedList(const Node* node) const {
    for (const auto n: m_closedList) {
        if (n->m_x == node->m_x && n->m_y == node->m_y) {
            return true;
        }
    }

    return false;
}

bool BFS::isInClosedList(const int x, const int y) const {
    for (const auto n: m_closedList) {
        if (n->m_x == x && n->m_y == y) {
            return true;
        }
    }

    return false;
}

void BFS::freeRemainingOpenList() {
    while (!m_openList.empty()) {
        const Node* node = m_openList.front();
        m_openList.pop();
        delete node;
    }
}

void BFS::freeRemainingClosedList() {
    for (const auto node: m_closedList) {
        delete node;
    }

    m_closedList.clear();
}
