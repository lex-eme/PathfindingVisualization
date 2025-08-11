#include "Node.h"

Node::Node(Node* parent, const Action& action, const int x, const int y, const int depth)
    : m_parent(parent), m_action(action), m_x(x), m_y(y), m_depth(depth) {
}
