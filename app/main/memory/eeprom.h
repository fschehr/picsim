#ifndef EEPROM_H
#define EEPROM_H

#include <vector>
#include <functional>
#include <string>
#include <map>
#include <mutex>

template <typename T>
class EepromMemory {
public:
    explicit EepromMemory(size_t size);
    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener);
    void removePropertyChangeListener(const std::string& propertyName);
    T get(int address) const;
    void set(int address, const T& toSet);

private:
    std::vector<T> memory;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    mutable std::mutex mutex;
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue);
};

#endif // EEPROM_H
