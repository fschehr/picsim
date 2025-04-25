#ifndef RAM_H
#define RAM_H

#include <vector>
#include <functional>
#include <string>
#include <map>
#include <mutex>

template <typename T>
class RamMemory {
public:
    enum class Bank { BANK_0, BANK_1 };

    struct SFR {
        Bank bank;
        int address;
        bool mapped;
        static SFR valueOf(Bank bank, int address);
        static const std::vector<SFR>& entries();
    };

    RamMemory();
    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener);
    void removePropertyChangeListener(const std::string& propertyName);
    T get(int address) const;
    T get(Bank bank, int address) const;
    void set(Bank bank, int address, const T& value);
    void set(const SFR& sfr, const T& value);
    T get(const SFR& sfr) const;

private:
    static const int BANK_SIZE = 128;
    std::vector<T> bank0;
    std::vector<T> bank1;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    mutable std::mutex mutex;
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue);
};

#endif // RAM_H
