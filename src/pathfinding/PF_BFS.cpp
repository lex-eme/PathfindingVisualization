#include "PF_BFS.h"

#include <iostream>


PF_BFS::PF_BFS(WorldMap& map): PF(map) {
    m_nodes.reserve(1024);
}

PF_BFS::~PF_BFS() = default;

void PF_BFS::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_goalX = gx;
    m_goalY = gy;

    m_inProgress = true;
    m_openList.clear();
    m_closedList.clear();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList.push(m_node);
}

void PF_BFS::searchIteration() {
    if (!m_inProgress) {
        return;
    }

    if (m_openList.empty()) {
        m_inProgress = false;
        std::cout << "Unreachable destination" << std::endl;
        return;
    }

    m_node = m_openList.pop();

    if (m_node->m_x == m_goalX && m_node->m_y == m_goalY) {
        m_inProgress = false;
        std::cout << "Path found" << std::endl;
        std::cout << "Open list: " << m_openList.m_count << std::endl;
        std::cout << "Closed list: " << m_closedList.size() << std::endl;
        std::cout << "Path length: " << m_node->m_depth << std::endl;
        return;
    }

    m_closedList.push_back(m_node);

    expand();
}

std::vector<Action> PF_BFS::getClosedList() const {
    std::vector<Action> result;
    result.reserve(m_closedList.size());

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF_BFS::getOpenList() const {
    std::vector<Action> result;

    for (size_t i = 0; i < m_openList.m_count; i++) {
        const size_t index = (m_openList.m_start + i) % m_openList.m_length;
        result.push_back({m_openList.m_nodes[index]->m_x, m_openList.m_nodes[index]->m_y});
    }

    return result;
}

std::vector<Action> PF_BFS::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

void PF_BFS::expand() {
    // ACTIONS                                  UP            DOWN                  LEFT            RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (auto action: actions) {
        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;
            if (!isInClosedList(newX, newY) && !isInOpenList(newX, newY)) {
                m_nodes.emplace_back(m_node, action, newX, newY, m_node->m_depth + 1);
                m_openList.push(&m_nodes.back());
            }
        }
    }
}

bool PF_BFS::isInClosedList(const int x, const int y) const {
    return std::any_of(m_closedList.begin(), m_closedList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

bool PF_BFS::isInOpenList(const int x, const int y) const {
    for (size_t i = 0; i < m_openList.m_count; i++) {
        const size_t index = (m_openList.m_start + i) % m_openList.m_length;
        if (m_openList.m_nodes[index]->m_x == x && m_openList.m_nodes[index]->m_y == y) {
            return true;
        }
    }

    return false;
}
