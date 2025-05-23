#include "ram.h"
#include <stdexcept>
template <typename T>
typename RamMemory<T>::SFR RamMemory<T>::SFR::valueOf(Bank bank, int address) {
    for (const auto& sfr : entries()) {
        if ((sfr.mapped && address == sfr.address) || (sfr.bank == bank && address == sfr.address)) {
            return sfr;
        }
    }
    throw std::invalid_argument("No such address found");
}

template <>
const std::vector<RamMemory<int>::SFR>& RamMemory<int>::SFR::entries() {
    static const std::vector<SFR> sfrEntries = {
        {Bank::BANK_0, 0x00, true},  // INDF - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x01, false}, // TMR0 - nur in Bank 0
        {Bank::BANK_0, 0x02, true},  // PCL - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x03, true},  // STATUS - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x04, true},  // FSR - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x05, false}, // PORTA - nur in Bank 0
        {Bank::BANK_0, 0x06, false}, // PORTB - nur in Bank 0
        {Bank::BANK_0, 0x08, false}, // EEDATA - nur in Bank 0
        {Bank::BANK_0, 0x09, false}, // EEADR - nur in Bank 0
        {Bank::BANK_0, 0x0A, true},  // PCLATH - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x0B, true},  // INTCON - in beiden Bänken gespiegelt
        {Bank::BANK_1, 0x01, false}, // OPTION_REG - nur in Bank 1
        {Bank::BANK_1, 0x05, false}, // TRISA - nur in Bank 1
        {Bank::BANK_1, 0x06, false}, // TRISB - nur in Bank 1
        {Bank::BANK_1, 0x08, false}, // EECON1 - nur in Bank 1
        {Bank::BANK_1, 0x09, false}, // EECON2 - nur in Bank 1
    };
    return sfrEntries;
}

template <>
const std::vector<RamMemory<uint8_t>::SFR>& RamMemory<uint8_t>::SFR::entries() {
    static const std::vector<SFR> sfrEntries = {
        {Bank::BANK_0, 0x00, true},  // INDF - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x01, false}, // TMR0 - nur in Bank 0
        {Bank::BANK_0, 0x02, true},  // PCL - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x03, true},  // STATUS - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x04, true},  // FSR - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x05, false}, // PORTA - nur in Bank 0
        {Bank::BANK_0, 0x06, false}, // PORTB - nur in Bank 0
        {Bank::BANK_0, 0x08, false}, // EEDATA - nur in Bank 0
        {Bank::BANK_0, 0x09, false}, // EEADR - nur in Bank 0
        {Bank::BANK_0, 0x0A, true},  // PCLATH - in beiden Bänken gespiegelt
        {Bank::BANK_0, 0x0B, true},  // INTCON - in beiden Bänken gespiegelt
        {Bank::BANK_1, 0x01, false}, // OPTION_REG - nur in Bank 1
        {Bank::BANK_1, 0x05, false}, // TRISA - nur in Bank 1
        {Bank::BANK_1, 0x06, false}, // TRISB - nur in Bank 1
        {Bank::BANK_1, 0x08, false}, // EECON1 - nur in Bank 1
        {Bank::BANK_1, 0x09, false}, // EECON2 - nur in Bank 1
    };
    return sfrEntries;
}

template <typename T>
RamMemory<T>::RamMemory(int BANK_SIZE) : bank0(BANK_SIZE), bank1(BANK_SIZE) {}

template <typename T>
void RamMemory<T>::addPropertyChangeListener(const std::string& propertyName, std::function<void(int, T, T)> listener) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners[propertyName] = listener;
}
template <typename T>
void RamMemory<T>::removePropertyChangeListener(const std::string& propertyName) {
    std::lock_guard<std::mutex> lock(mutex);
    propertyChangeListeners.erase(propertyName);
}

template <typename T>
T RamMemory<T>::get(int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (address < 0 || address >= bank0.size() * 2) {
        throw std::out_of_range("Address isn't implemented");
    }
    return (address < bank0.size()) ? bank0[address] : bank1[address - bank0.size()];
}

template <typename T>
T RamMemory<T>::get(Bank bank, int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (address < 0 || address >= bank0.size()) {
        throw std::out_of_range("Address isn't implemented");
    }
    return (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
}

template <typename T>
void RamMemory<T>::set(Bank bank, int address, const T& value) {
    std::lock_guard<std::mutex> lock(mutex);
    if (address < 0 || address >= bank0.size()) {
        throw std::out_of_range("Address isn't implemented");
    }
    Logger::info("set value: " + std::to_string(value)+ " to address: " + std::to_string(address));
    int oldValue = (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
    
    // Prüfen, ob es sich um ein SFR handelt
    bool isSFR = address < 0x0C;
    bool shouldMirror = false;
    
    // Für SFRs: Nur spiegeln, wenn das Register in beiden Bänken verfügbar ist
    if (isSFR) {
        // Prüfen, ob das SFR an dieser Adresse gespiegelt ist
        for (const auto& sfr : SFR::entries()) {
            if (sfr.address == address && (sfr.bank == bank)) {
                shouldMirror = sfr.mapped;
                break;
            }
        }
    } else {
        // Für GPRs: Immer spiegeln (vollständige Spiegelung ab 0x0C)
        shouldMirror = true;
    }
    
    if (bank == Bank::BANK_0) {
        bank0[address] = value;
        if (shouldMirror) {
            bank1[address] = value; // Spiegelung zur Bank 1
        }
    } else {
        bank1[address] = value;
        if (shouldMirror) {
            bank0[address] = value; // Spiegelung zur Bank 0
        }
    }
    
    firePropertyChange("ram", address, oldValue, value);
}

template <typename T>
void RamMemory<T>::set(const SFR &sfr, const T &value) {
    set(sfr.bank, sfr.address, value);
}


template <typename T>
T RamMemory<T>::get(const SFR &sfr) const
{
    return get(sfr.bank, sfr.address);
}

// template <typename T>
// void RamMemory<T>::clear() {
//     *this = RamMemory<uint8_t> (bank0.size);
// }


template <typename T>
void RamMemory<T>::firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
    if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
        propertyChangeListeners[propertyName](index, oldValue, newValue);
    }
}

template class RamMemory<uint8_t>;