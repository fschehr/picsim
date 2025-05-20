#include "program.h"
#include <stdexcept>
#include <mutex>
#include <iostream>

// Constructor
template <typename T>
ProgramMemory<T>::ProgramMemory(size_t size) : memory(size) {}

// Add property change listener
template <typename T>
void ProgramMemory<T>::addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners[propertyName] = listener;
}

// Remove property change listener
template <typename T>
void ProgramMemory<T>::removePropertyChangeListener(const std::string& propertyName) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners.erase(propertyName);
}

// Get value from memory
template <typename T>
T ProgramMemory<T>::get(int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
        throw std::out_of_range("Address out of bounds");
    }
    return memory[address];
}

// Set value in memory
template <typename T>
void ProgramMemory<T>::set(int address, const T& toSet) {
    std::lock_guard<std::mutex> lock(mutex);
    if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
        throw std::out_of_range("Address out of bounds");
    } else {
        T beforeSet = memory[address];
        memory[address] = toSet;
        firePropertyChange("memory", address, beforeSet, toSet);
    }
}

// Fire property change event
template <typename T>
void ProgramMemory<T>::firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
    if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
        propertyChangeListeners[propertyName](index, oldValue, newValue);
    }
}

template class ProgramMemory<uint16_t>;