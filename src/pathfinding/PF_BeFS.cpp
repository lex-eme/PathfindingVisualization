#include "PF_BeFS.h"

#include <iostream>

NodePriorityQueue::NodePriorityQueue() {
    m_nodes = new Node*[m_length];
}

NodePriorityQueue::~NodePriorityQueue() {
    delete m_nodes;
}

bool NodePriorityQueue::empty() const {
    return m_count == 0;
}

void NodePriorityQueue::push(Node* node) {
    if (m_count >= m_length) {
        std::cerr << "NodePriorityQueue is full" << std::endl;
        return;
    }

    m_nodes[m_count] = node;
    m_count += 1;

    size_t nodeIndex = m_count;
    size_t parentIndex = parent(nodeIndex);

    while (nodeIndex > 1 && m_nodes[parentIndex - 1]->m_heuristic > m_nodes[nodeIndex - 1]->m_heuristic) {
        Node* t = m_nodes[parentIndex - 1];
        m_nodes[parentIndex - 1] = m_nodes[nodeIndex - 1];
        m_nodes[nodeIndex - 1] = t;
        nodeIndex = parentIndex;
        parentIndex = parent(nodeIndex);
    }
}

Node* NodePriorityQueue::pop() {
    if (m_count < 1) {
        std::cerr << "NodePriorityQueue is empty" << std::endl;
        return nullptr;
    }

    Node* front = m_nodes[0];

    m_count -= 1;
    m_nodes[0] = m_nodes[m_count];

    size_t nodeIndex = 1;
    size_t minIndex = nodeIndex;

    do {
        nodeIndex = minIndex;

        if (const size_t leftIndex = left(nodeIndex);
            leftIndex <= m_count && m_nodes[leftIndex - 1]->m_heuristic < m_nodes[minIndex - 1]->m_heuristic) {
            minIndex = leftIndex;
        }

        if (const size_t rightIndex = right(nodeIndex);
            rightIndex <= m_count && m_nodes[rightIndex - 1]->m_heuristic < m_nodes[minIndex - 1]->m_heuristic) {
            minIndex = rightIndex;
        }

        if (minIndex != nodeIndex) {
            Node* t = m_nodes[minIndex - 1];
            m_nodes[minIndex - 1] = m_nodes[nodeIndex - 1];
            m_nodes[nodeIndex - 1] = t;
        }
    } while (minIndex != nodeIndex);

    return front;
}

void NodePriorityQueue::clear() {
    m_count = 0;
}

inline size_t NodePriorityQueue::left(const size_t index) {
    return index * 2;
}

inline size_t NodePriorityQueue::right(const size_t index) {
    return index * 2 + 1;
}

inline size_t NodePriorityQueue::parent(const size_t index) {
    return index / 2;
}

PF_BeFS::PF_BeFS(WorldMap& map) : PF(map) {
}

PF_BeFS::~PF_BeFS() {
    freeRemainingOpenList();
    freeRemainingClosedList();
}

void PF_BeFS::startSearch(const int sx, const int sy, const int gx, const int gy) {
    m_goalX = gx;
    m_goalY = gy;

    m_inProgress = true;
    freeRemainingOpenList();
    freeRemainingClosedList();

    m_node = new Node(nullptr, {0, 0}, sx, sy, 0);
    m_openList.push(m_node);
}

void PF_BeFS::searchIteration() {
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
        const size_t createdNodeCount = m_openList.m_count + m_closedList.size();
        std::cout << "Nodes created: " << createdNodeCount << std::endl;
        return;
    }

    m_closedList.push_back(m_node);

    expand();
}

std::vector<Action> PF_BeFS::getClosedList() const {
    std::vector<Action> result;
    result.reserve(m_closedList.size());

    for (const auto node: m_closedList) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

std::vector<Action> PF_BeFS::getOpenList() const {
    std::vector<Action> result;
    result.reserve(m_openList.m_count);

    for (size_t i = 0; i < m_openList.m_count; i++) {
        result.push_back({m_openList.m_nodes[i]->m_x, m_openList.m_nodes[i]->m_y});
    }

    return result;
}

std::vector<Action> PF_BeFS::getPath() const {
    std::vector<Action> result;

    const Node* node = m_node;
    while (node->m_parent != nullptr) {
        result.push_back({node->m_x, node->m_y});
        node = node->m_parent;
    }

    return result;
}

void PF_BeFS::expand() {
    // ACTIONS                                  UP            DOWN                  LEFT            RIGHT
    const static std::vector<Action> actions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (auto action: actions) {
        if (m_map.isLegalAction(m_node->m_x, m_node->m_y, action)) {
            const int newX = m_node->m_x + action.x;
            const int newY = m_node->m_y + action.y;

            int heuristic = abs(m_goalX - newX) + abs(m_goalY - newY);

            if (!isInClosedList(newX, newY) && !isInOpenList(newX, newY)) {
                const auto newNode = new Node(m_node, action, newX, newY, m_node->m_depth + 1, heuristic);
                m_openList.push(newNode);
            }
        }
    }
}

bool PF_BeFS::isInClosedList(int x, int y) const {
    return std::any_of(m_closedList.begin(), m_closedList.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

bool PF_BeFS::isInOpenList(const int x, const int y) const {
    for (size_t i = 0; i < m_openList.m_count; i++) {
        if (m_openList.m_nodes[i]->m_x == x && m_openList.m_nodes[i]->m_y == y) {
            return true;
        }
    }

    return false;
}

void PF_BeFS::freeRemainingOpenList() {
    for (size_t i = 0; i < m_openList.m_count; i++) {
        const Node* node = m_openList.m_nodes[i];
        delete node;
    }

    m_openList.clear();
}

void PF_BeFS::freeRemainingClosedList() {
    for (const auto node: m_closedList) {
        delete node;
    }

    m_closedList.clear();
}
