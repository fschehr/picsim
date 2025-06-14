#ifndef RAM_H
#define RAM_H

#include <vector>
#include <functional>
#include <string>
#include <map>
#include <mutex>
#include "../logger.h"

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

    RamMemory(int BANK_SIZE);
    void addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener);
    void removePropertyChangeListener(const std::string& propertyName);
    void setPCLUpdateCallback(std::function<void(T)> callback);
    T get(int address) const;
    T get(Bank bank, int address) const;
    void set(Bank bank, int address, const T& value);
    void set(const SFR& sfr, const T& value);
    T get(const SFR& sfr) const;
    void clear();
    bool initializing = true;;
private:
    std::vector<T> bank0;
    std::vector<T> bank1;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    std::function<void(T)> pclUpdateCallback;
    mutable std::mutex mutex;
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue);
    bool checkSFRMirroring(Bank bank, int address) const;
    void handleIndirectSet(Bank bank, const T& value);
};

#endif // RAM_H
