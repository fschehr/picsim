#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>
#include <mutex>
#include "../logger.h"

template <typename T>
class StackMemory {
public:
    explicit StackMemory(size_t size);
    void push(const T& value);
    T pop();
    T peek() const;
    bool isEmpty() const;
    bool isFull() const;
    void clear();
    std::vector<T> getStackContents() const ;
    T getValueAt(size_t index) const {
        std::lock_guard<std::mutex> lock(mutex);
        if (index >= memory.size()) {
            throw std::out_of_range("Index out of range");
        }
        return memory[index];
    }

    int getCurrentTop() const {
        std::lock_guard<std::mutex> lock(mutex);
        return top;
    }
private:
    std::vector<T> memory;
    int top;
    mutable std::mutex mutex;
    const size_t initial_size; // Make initial_size const
};

#endif // STACK_H
