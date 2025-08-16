#pragma once

#include <vector>

struct Action {
    int x;
    int y;
};

struct Node {
    Node* m_parent = nullptr;
    Action m_action = {0, 0};
    int m_x = 0;
    int m_y = 0;
    int m_depth = 0;
    int m_heuristic = 0;

    Node() = default;
    Node(Node* parent, const Action& action, int x, int y, int depth);
    Node(Node* parent, const Action& action, int x, int y, int depth, int heuristic);
};
