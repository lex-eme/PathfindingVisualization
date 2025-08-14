#include "PF_DFS.h"

#include <iostream>

PF_DFS::PF_DFS(WorldMap& map): PF(map) {
}

PF_DFS::~PF_DFS() {
    freeRemainingOpenList();
    freeRemainingClosedList();
}

void PF_DFS::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_goalX = gx;
    m_goalY = gy;

    m_inProgress = true;
    freeRemainingOpenList();
    freeRemainingClosedList();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList.push_back(m_node);
}

void PF_DFS::searchIteration() {
    if (!m_inProgress) {
        return;
    }

    if (m_openList.empty()) {
        m_inProgress = false;
        std::cout << "Unreachable destination" << std::endl;
        return;
    }

    m_node = m_openList.back();
    m_openList.pop_back();


    if (m_node->m_x == m_goalX && m_node->m_y == m_goalY) {
        m_inProgress = false;
        std::cout << "Path found" << std::endl;
        const size_t createdNodeCount = m_openList.size() + m_closedList.size();
        std::cout << "Nodes created: " << createdNodeCount << std::endl;
        return;
    }

    m_closedList.push_back(m_node);

    expand();
}

std::vector<Action> PF_DFS::getClosedList() const {
    std::vector<Action> result;
    result.reserve(m_closedList.size());

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF_DFS::getOpenList() const {
    std::vector<Action> result;
    result.reserve(m_openList.size());

    for (const auto node: m_openList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF_DFS::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

void PF_DFS::expand() {
    // ACTIONS                                  UP            DOWN                  LEFT            RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (auto action: actions) {
        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;
            if (!isInClosedList(newX, newY) && !isInOpenList(newX, newY)) {
                const auto newNode = new Node(m_node, action, newX, newY, m_node->m_depth + 1);
                m_openList.push_back(newNode);
            }
        }
    }
}

bool PF_DFS::isInClosedList(const int x, const int y) const {
    return std::any_of(m_closedList.begin(), m_closedList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

bool PF_DFS::isInOpenList(const int x, const int y) const {
    return std::any_of(m_openList.begin(), m_openList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

void PF_DFS::freeRemainingOpenList() {
    for (const auto node: m_openList) {
        delete node;
    }

    m_openList.clear();
}

void PF_DFS::freeRemainingClosedList() {
    for (const auto node: m_closedList) {
        delete node;
    }

    m_closedList.clear();
}
