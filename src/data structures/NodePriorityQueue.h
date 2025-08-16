#pragma once
#include "../Node.h"


class PF_BeFS;
class PF_AStar;

class NodePriorityQueue {
    friend PF_BeFS;
    friend PF_AStar;

    Node** m_nodes;
    size_t m_length;
    size_t m_count = 0;

public:
    explicit NodePriorityQueue(size_t maxSize);
    ~NodePriorityQueue();

    [[nodiscard]] bool empty() const;

    void push(Node* node);
    Node* pop();
    void clear();

private:
    [[nodiscard]] static size_t left(size_t index);
    [[nodiscard]] static size_t right(size_t index);
    [[nodiscard]] static size_t parent(size_t index);
};
