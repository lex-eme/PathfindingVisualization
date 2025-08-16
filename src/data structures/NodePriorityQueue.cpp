#include "NodePriorityQueue.h"

#include <iostream>


NodePriorityQueue::NodePriorityQueue(const size_t maxSize)
    : m_length(maxSize) {
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
