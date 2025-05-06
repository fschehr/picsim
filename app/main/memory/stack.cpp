#include "stack.h"

// Constructor
template <typename T>
StackMemory<T>::StackMemory(size_t size) : memory(size), pointer(-1) {}

// Add property change listener
template <typename T>
void StackMemory<T>::addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners[propertyName] = listener;
}

// Remove property change listener
template <typename T>
void StackMemory<T>::removePropertyChangeListener(const std::string& propertyName) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners.erase(propertyName);
}

// Get value from stack
template <typename T>
T StackMemory<T>::get(int address) {
    std::lock_guard<std::mutex> lock(mutex);
    if (address < 0 || address >= static_cast<int>(memory.size())) {
        throw std::out_of_range("Stack contains only eight levels");
    }
    return memory[address];
}

// Push value onto stack
template <typename T>
void StackMemory<T>::push(const T& value) {
    std::lock_guard<std::mutex> lock(mutex);
    if (isFull()) {
        throw std::out_of_range("Stack overflow detected, stack is full");
    }
    memory[++pointer] = value;
    firePropertyChange("memory", pointer, T(), value);
}

// Pop value from stack
template <typename T>
T StackMemory<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex);
    if (isEmpty()) {
        throw std::out_of_range("Stack underflow");
    }
    T oldValue = memory[pointer];
    firePropertyChange("memory", pointer, oldValue, T());
    return memory[pointer--];
}

// Get top value from stack
template <typename T>
T StackMemory<T>::top() {
    std::lock_guard<std::mutex> lock(mutex);
    if (isEmpty()) {
        throw std::out_of_range("Stack underflow detected, stack is empty");
    }
    return memory[pointer];
}

// Check if stack is full
template <typename T>
bool StackMemory<T>::isFull() const {
    std::lock_guard<std::mutex> lock(mutex);
    return pointer == static_cast<int>(memory.size()) - 1;
}

// Check if stack is empty
template <typename T>
bool StackMemory<T>::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return pointer == -1;
}

// Fire property change event
template <typename T>
void StackMemory<T>::firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
    if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
        propertyChangeListeners[propertyName](index, oldValue, newValue);
    }
}