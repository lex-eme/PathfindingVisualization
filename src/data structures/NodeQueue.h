#pragma once
#include "../Node.h"

class PF_BFS;

class NodeQueue {
    friend PF_BFS;

    Node** m_nodes;
    size_t m_length = 64;
    size_t m_start = 0;
    size_t m_end = 0;
    size_t m_count = 0;

public:
    NodeQueue();
    ~NodeQueue();

    [[nodiscard]] bool empty() const;

    void push(Node* node);
    Node* pop();
    void clear();
};
