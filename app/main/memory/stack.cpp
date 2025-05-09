#include "stack.h"
#include <stdexcept>
#include <mutex>

template <typename T>
StackMemory<T>::StackMemory(size_t size) : memory(size), top(-1) {}

template <typename T>
void StackMemory<T>::push(const T& value) {
    std::lock_guard<std::mutex> lock(mutex);
    if (top >= static_cast<int>(memory.size()) - 1) {
        throw std::out_of_range("Stack overflow");
    }
    memory[++top] = value;
}

template <typename T>
T StackMemory<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex);
    if (top < 0) {
        throw std::out_of_range("Stack underflow");
    }
    return memory[top--];
}

template <typename T>
T StackMemory<T>::peek() const {
    std::lock_guard<std::mutex> lock(mutex);
    if (top < 0) {
        throw std::out_of_range("Stack is empty");
    }
    return memory[top];
}

template <typename T>
bool StackMemory<T>::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return top == -1;
}

template <typename T>
bool StackMemory<T>::isFull() const {
    std::lock_guard<std::mutex> lock(mutex);
    return top == static_cast<int>(memory.size()) - 1;
}

template class StackMemory<int>;