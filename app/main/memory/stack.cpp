#include <vector>
#include <stdexcept>
#include <mutex>
#include <functional>
#include <string>
#include <map>

template <typename T>
class StackMemory {
private:
    std::vector<T> memory;
    int pointer;
    std::mutex mutex;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;

public:
    explicit StackMemory(size_t size) : memory(size), pointer(-1) {}

    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
        std::lock_guard<std::mutex> lock(mutex);
        propertyChangeListeners[propertyName] = listener;
    }

    void removePropertyChangeListener(const std::string& propertyName) {
        std::lock_guard<std::mutex> lock(mutex);
        propertyChangeListeners.erase(propertyName);
    }

    T get(int address) {
        std::lock_guard<std::mutex> lock(mutex);
        if (address < 0 || address >= static_cast<int>(memory.size())) {
            throw std::out_of_range("Stack contains only eight levels");
        }
        return memory[address];
    }

    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (isFull()) {
            throw std::out_of_range("Stack overflow detected, stack is full");
        }
        memory[++pointer] = value;
        firePropertyChange("memory", pointer, T(), value);
    }

    T pop() {
        std::lock_guard<std::mutex> lock(mutex);
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow");
        }
        T oldValue = memory[pointer];
        firePropertyChange("memory", pointer, oldValue, T());
        return memory[pointer--];
    }

    T top() {
        std::lock_guard<std::mutex> lock(mutex);
        if (isEmpty()) {
            throw std::out_of_range("Stack underflow detected, stack is empty");
        }
        return memory[pointer];
    }

    bool isFull() const {
        std::lock_guard<std::mutex> lock(mutex);
        return pointer == static_cast<int>(memory.size()) - 1;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return pointer == -1;
    }

private:
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
        if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
            propertyChangeListeners[propertyName](index, oldValue, newValue);
        }
    }
};