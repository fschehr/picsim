#include "ram.h"
#include <stdexcept>
#include <functional>
#include <mutex>
#include <map>
#include <vector>

template <typename T>
class RamMemory {
public:
    enum class Bank {
        BANK_0,
        BANK_1,
    };

    struct SFR {
        Bank bank;
        int address;
        bool mapped;

        static SFR valueOf(Bank bank, int address) {
            for (const auto& sfr : entries()) {
                if ((sfr.mapped && address == sfr.address) || (sfr.bank == bank && address == sfr.address)) {
                    return sfr;
                }
            }
            throw std::invalid_argument("No such address found");
        }

        static const std::vector<SFR>& entries() {
            static const std::vector<SFR> sfrEntries = {
                {Bank::BANK_0, 0x00, true},  // INDF
                {Bank::BANK_0, 0x01, false}, // TMR0
                {Bank::BANK_0, 0x02, true},  // PCL
                {Bank::BANK_0, 0x03, true},  // STATUS
                {Bank::BANK_0, 0x04, true},  // FSR
                {Bank::BANK_0, 0x05, false}, // PORTA
                {Bank::BANK_0, 0x06, false}, // PORTB
                {Bank::BANK_0, 0x08, false}, // EEDATA
                {Bank::BANK_0, 0x09, false}, // EEADR
                {Bank::BANK_0, 0x0A, true},  // PCLATH
                {Bank::BANK_0, 0x0B, true},  // INTCON
                {Bank::BANK_1, 0x01, false}, // OPTION
                {Bank::BANK_1, 0x05, false}, // TRISA
                {Bank::BANK_1, 0x06, false}, // TRISB
                {Bank::BANK_1, 0x08, false}, // EECON1
                {Bank::BANK_1, 0x09, false}, // EECON2
            };
            return sfrEntries;
        }
    };

private:
    std::vector<T> bank0;
    std::vector<T> bank1;
    std::map<std::string, std::function<void(int, T, T)>> propertyChangeListeners;
    mutable std::mutex mutex;

public:
    RamMemory(int BANK_SIZE) : bank0(BANK_SIZE), bank1(BANK_SIZE) {}

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
        if (address < 0 || address >= BANK_SIZE * 2) {
            throw std::out_of_range("Address isn't implemented");
        }
        return (address < BANK_SIZE) ? bank0[address] : bank1[address - BANK_SIZE];
    }

    T get(Bank bank, int address) const {
        std::lock_guard<std::mutex> lock(mutex);
        if (address < 0 || address >= BANK_SIZE) {
            throw std::out_of_range("Address isn't implemented");
        }
        return (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
    }

    void set(Bank bank, int address, const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (address < 0 || address >= BANK_SIZE) {
            throw std::out_of_range("Address isn't implemented");
        }

        T oldValue = (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
        if (bank == Bank::BANK_0) {
            bank0[address] = value;
            if (address < 0x0C) {
                bank1[address] = value; // Mapped to second bank
            }
        } else {
            bank1[address] = value;
            if (address < 0x0C) {
                bank0[address] = value; // Mapped to first bank
            }
        }
        firePropertyChange("ram", address, oldValue, value);
    }

    void set(const SFR& sfr, const T& value) {
        set(sfr.bank, sfr.address, value);
    }

    T get(const SFR& sfr) const {
        return get(sfr.bank, sfr.address);
    }

private:
    void firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
        if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
            propertyChangeListeners[propertyName](index, oldValue, newValue);
        }
    }
};