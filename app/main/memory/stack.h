#ifndef STACK_H
#define STACK_H

#include <vector>
#include <functional>
#include <string>
#include <map>
#include <mutex>

template <typename T>
class StackMemory {
public:
    explicit StackMemory(size_t size);
    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener);
    void removePropertyChangeListener(const std::string& propertyName);
    T get(int address);
    void push(const T& value);
    T pop();
    T top();
    bool isFull() const;
    bool isEmpty() const;

private:
    std::vector<T> memory;
    int pointer;
    std::mutex mutex;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue);
};

#endif // STACK_H
