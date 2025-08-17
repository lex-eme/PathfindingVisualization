#include "OpenList.h"

void StackOpenList::push(Node* node) {
    m_stack.push_back(node);
}

Node* StackOpenList::pop() {
    Node* back = m_stack.back();
    m_stack.pop_back();
    return back;
}

void StackOpenList::clear() {
    m_stack.clear();
}

bool StackOpenList::empty() const {
    return m_stack.empty();
}

bool StackOpenList::containsState(int x, int y) const {
    return std::any_of(m_stack.begin(), m_stack.end(), [x, y](const Node* n) {
        return n->m_x == x && n->m_y == y;
    });
}

size_t StackOpenList::size() const {
    return m_stack.size();
}

std::vector<Action> StackOpenList::getStates() const {
    std::vector<Action> result;
    result.reserve(m_stack.size());

    for (const auto node: m_stack) {
        result.push_back({node->m_x, node->m_y});
    }

    return result;
}

PriorityQueueOpenList::PriorityQueueOpenList()
    : m_queue(512) {
}

void PriorityQueueOpenList::push(Node* node) {
    m_queue.push(node);
}

Node* PriorityQueueOpenList::pop() {
    return m_queue.pop();
}

void PriorityQueueOpenList::clear() {
    m_queue.clear();
}

bool PriorityQueueOpenList::empty() const {
    return m_queue.empty();
}

bool PriorityQueueOpenList::containsState(int x, int y) const {
    for (size_t i = 0; i < m_queue.m_count; i++) {
        if (m_queue.m_nodes[i]->m_x == x && m_queue.m_nodes[i]->m_y == y) {
            return true;
        }
    }

    return false;
}

size_t PriorityQueueOpenList::size() const {
    return m_queue.m_count;
}

std::vector<Action> PriorityQueueOpenList::getStates() const {
    std::vector<Action> result;
    result.reserve(m_queue.m_count);

    for (size_t i = 0; i < m_queue.m_count; i++) {
        result.push_back({m_queue.m_nodes[i]->m_x, m_queue.m_nodes[i]->m_y});
    }

    return result;
}

void QueueOpenList::push(Node* node) {
    m_queue.push(node);
}

Node* QueueOpenList::pop() {
    return m_queue.pop();
}

void QueueOpenList::clear() {
    m_queue.clear();
}

bool QueueOpenList::empty() const {
    return m_queue.empty();
}

bool QueueOpenList::containsState(int x, int y) const {
    for (size_t i = 0; i < m_queue.m_count; i++) {
        const size_t index = (m_queue.m_start + i) % m_queue.m_capacity;
        if (m_queue.m_nodes[index]->m_x == x && m_queue.m_nodes[index]->m_y == y) {
            return true;
        }
    }

    return false;
}

size_t QueueOpenList::size() const {
    return m_queue.m_count;
}

std::vector<Action> QueueOpenList::getStates() const {
    std::vector<Action> result;

    for (size_t i = 0; i < m_queue.m_count; i++) {
        const size_t index = (m_queue.m_start + i) % m_queue.m_capacity;
        result.push_back({m_queue.m_nodes[index]->m_x, m_queue.m_nodes[index]->m_y});
    }

    return result;
}
