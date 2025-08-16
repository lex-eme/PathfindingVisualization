#include "NodeQueue.h"

#include <iostream>


NodeQueue::NodeQueue() {
    m_nodes = new Node*[m_length];
}

NodeQueue::~NodeQueue() {
    delete m_nodes;
}

void NodeQueue::push(Node* node) {
    if (m_count >= m_length) {
        std::cerr << "QueueNode is full." << std::endl;
        return;
    }

    m_nodes[m_end] = node;
    m_count += 1;
    m_end = (m_end + 1) % m_length;
}

Node* NodeQueue::pop() {
    if (m_count < 1) {
        std::cerr << "QueueNode is empty" << std::endl;
        return nullptr;
    }
    Node* node = m_nodes[m_start];
    m_start = (m_start + 1) % m_length;
    m_count -= 1;
    return node;
}

void NodeQueue::clear() {
    m_count = 0;
    m_start = 0;
    m_end = 0;
}

bool NodeQueue::empty() const {
    return m_count == 0;
}
