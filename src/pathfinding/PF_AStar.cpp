#include "PF_AStar.h"

#include <iostream>

PF_AStar::PF_AStar(WorldMap& map)
    : PF(map), m_openList(512) {
}

PF_AStar::~PF_AStar() {
    freeRemainingOpenList();
    freeRemainingClosedList();
}

void PF_AStar::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_goalX = gx;
    m_goalY = gy;

    m_inProgress = true;
    freeRemainingOpenList();
    freeRemainingClosedList();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList.push(m_node);
}

void PF_AStar::searchIteration() {
    if (!m_inProgress) {
        return;
    }

    if (m_openList.empty()) {
        m_inProgress = false;
        std::cout << "Unreachable destination" << std::endl;
        return;
    }

    m_node = m_openList.pop();

    if (isInClosedList(m_node->m_x, m_node->m_y)) {
        return;
    }

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

std::vector<Action> PF_AStar::getClosedList() const {
    std::vector<Action> result;
    result.reserve(m_closedList.size());

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF_AStar::getOpenList() const {
    std::vector<Action> result;
    result.reserve(m_openList.m_count);

    for (size_t i = 0; i < m_openList.m_count; i++) {
        result.push_back({m_openList.m_nodes[i]->m_x, m_openList.m_nodes[i]->m_y});
    }

    return result;
}

std::vector<Action> PF_AStar::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

void PF_AStar::expand() {
    // ACTIONS                                  UP            DOWN                  LEFT            RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (auto action: actions) {
        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;

            int heuristic = abs(m_goalX - newX) + abs(m_goalY - newY);

            if (!isInClosedList(newX, newY)) {
                const auto newNode = new Node(m_node, action, newX, newY, m_node->m_depth + 1, m_node->m_depth + heuristic);
                m_openList.push(newNode);
            }
        }
    }
}

bool PF_AStar::isInClosedList(int x, int y) const {
    return std::any_of(m_closedList.begin(), m_closedList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

bool PF_AStar::isInOpenList(const int x, const int y) const {
    for (size_t i = 0; i < m_openList.m_count; i++) {
        if (m_openList.m_nodes[i]->m_x == x && m_openList.m_nodes[i]->m_y == y) {
            return true;
        }
    }

    return false;
}

void PF_AStar::freeRemainingOpenList() {
    for (size_t i = 0; i < m_openList.m_count; i++) {
        const Node* node = m_openList.m_nodes[i];
        delete node;
    }

    m_openList.clear();
}

void PF_AStar::freeRemainingClosedList() {
    for (const auto node: m_closedList) {
        delete node;
    }

    m_closedList.clear();
}
