#pragma once
#include "Node.h"
#include "data structures/NodePriorityQueue.h"
#include "data structures/NodeQueue.h"


class OpenList {
public:
    virtual ~OpenList() = default;
    virtual void push(Node* node) = 0;
    virtual Node* pop() = 0;
    virtual void clear() = 0;

    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual bool containsState(int x, int y) const = 0;
    [[nodiscard]] virtual size_t size() const = 0;
    [[nodiscard]] virtual std::vector<Action> getStates() const = 0;
};

class StackOpenList final : public OpenList {
    std::vector<Node*> m_stack;

public:
    StackOpenList() = default;

    void push(Node* node) override;
    Node* pop() override;
    void clear() override;
    [[nodiscard]] bool empty() const override;
    [[nodiscard]] bool containsState(int x, int y) const override;
    [[nodiscard]] size_t size() const override;
    [[nodiscard]] std::vector<Action> getStates() const override;
};

class PriorityQueueOpenList final : public OpenList {
    NodePriorityQueue m_queue;

public:
    PriorityQueueOpenList();

    void push(Node* node) override;
    Node* pop() override;
    void clear() override;
    [[nodiscard]] bool empty() const override;
    [[nodiscard]] bool containsState(int x, int y) const override;
    [[nodiscard]] size_t size() const override;
    [[nodiscard]] std::vector<Action> getStates() const override;
};

class QueueOpenList final : public OpenList {
    NodeQueue m_queue;

public:
    QueueOpenList() = default;

    void push(Node* node) override;
    Node* pop() override;
    void clear() override;
    [[nodiscard]] bool empty() const override;
    [[nodiscard]] bool containsState(int x, int y) const override;
    [[nodiscard]] size_t size() const override;
    [[nodiscard]] std::vector<Action> getStates() const override;
};