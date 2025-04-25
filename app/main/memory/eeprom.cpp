#include "eeprom.h"
#include <stdexcept>
#include <functional>
#include <mutex>
#include <map>
#include <vector>

template <typename T>
class EepromMemory {
private:
    std::vector<T> memory;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    mutable std::mutex mutex;

public:
    explicit EepromMemory(size_t size) : memory(size) {}

    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
        std::lock_guard<std::mutex> lock(mutex);
        propertyChangeListeners[propertyName] = listener;
    }

    void removePropertyChangeListener(const std::string& propertyName) {
        std::lock_guard<std::mutex> lock(mutex);
        propertyChangeListeners.erase(propertyName);
    }

    T get(int address) const {
        std::lock_guard<std::mutex> lock(mutex);
        if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
            throw std::out_of_range("Address out of bounds");
        }
        return memory[address];
    }

    void set(int address, const T& toSet) {
        std::lock_guard<std::mutex> lock(mutex);
        if (memory.empty() || address >= static_cast<int>(memory.size()) || address < 0) {
            throw std::out_of_range("Address out of bounds");
        } else {
            T beforeSet = memory[address];
            memory[address] = toSet;
            firePropertyChange("memory", address, beforeSet, toSet);
        }
    }

private:
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
        if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
            propertyChangeListeners[propertyName](index, oldValue, newValue);
        }
    }
};