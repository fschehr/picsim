#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>
#include <mutex>

template <typename T>
class StackMemory {
public:
    explicit StackMemory(size_t size);
    void push(const T& value);
    T pop();
    T peek() const;
    bool isEmpty() const;
    bool isFull() const;

private:
    std::vector<T> memory;
    int top;
    mutable std::mutex mutex;
};

#endif // STACK_H
