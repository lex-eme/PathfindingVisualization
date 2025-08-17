#pragma once
#include "../Node.h"


class QueueOpenList;

class NodeQueue {
    friend QueueOpenList;

    Node** m_nodes;
    size_t m_capacity = 64;
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
