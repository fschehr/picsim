#include "eeprom.h"
#include <stdexcept>
#include <functional>
#include <mutex>
#include <map>
#include <vector>

template <typename T>
EepromMemory<T>::EepromMemory(size_t size) : memory(size) {}

template <typename T>
void EepromMemory<T>::addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners[propertyName] = listener;
}

template <typename T>
void EepromMemory<T>::removePropertyChangeListener(const std::string& propertyName) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners.erase(propertyName);
}

template <typename T>
T EepromMemory<T>::get(int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
        throw std::out_of_range("Address out of bounds");
    }
    return memory[address];
}

template <typename T>
void EepromMemory<T>::set(int address, const T& toSet) {
    std::lock_guard<std::mutex> lock(mutex);
    if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
        throw std::out_of_range("Address out of bounds");
    } else {
        T beforeSet = memory[address];
        memory[address] = toSet;
        firePropertyChange("memory", address, beforeSet, toSet);
    }
}

template <typename T>
void EepromMemory<T>::firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
    if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
        propertyChangeListeners[propertyName](index, oldValue, newValue);
    }
}