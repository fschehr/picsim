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
        {Bank::BANK_1, 0x88, false}, // EECON1 - nur in Bank 1 (korrekte Adresse)
        {Bank::BANK_1, 0x89, false}, // EECON2 - nur in Bank 1 (korrekte Adresse)
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
        {Bank::BANK_1, 0x88, false}, // EECON1 - nur in Bank 1 (korrekte Adresse)
        {Bank::BANK_1, 0x89, false}, // EECON2 - nur in Bank 1 (korrekte Adresse)
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
void RamMemory<T>::setPCLUpdateCallback(std::function<void(T)> callback) {
    std::lock_guard<std::mutex> lock(mutex);
    pclUpdateCallback = callback;
}

template <typename T>
T RamMemory<T>::get(int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    if (address < 0 || address >= bank0.size() * 2) {
        throw std::out_of_range("Address isn't implemented");
    }
    
    // Linear mapping for UI: 0-127 = bank0, 128-255 = bank1
    if (address < bank0.size()) {
        return bank0[address];
    } else {
        return bank1[address - bank0.size()];
    }
}

template <typename T>
T RamMemory<T>::get(Bank bank, int address) const {
    std::lock_guard<std::mutex> lock(mutex);
    //Logger::info("[RAM] Attempting to get value from bank " + std::to_string(static_cast<int>(bank)) + " address " + std::to_string(address));
    
    if (address < 0 || address >= bank0.size()) {
        Logger::error("[RAM] Invalid address access: " + std::to_string(address));
        throw std::out_of_range("Address isn't implemented");
    }    // Special handling for INDF register (indirect addressing)
    if(address == 0x00) {
        // FSR is mirrored, so read from the same bank we're accessing
        int fsr = (bank == Bank::BANK_0) ? bank0[0x04] : bank1[0x04];
        //Logger::info("[RAM] Indirect access through FSR: " + std::to_string(fsr));
        
        if (fsr == 0x00) {
            Logger::error("[RAM] Indirect addressing loop detected");
            throw std::logic_error("Indirect addressing loop detected");
        }
        if (fsr >= bank0.size()) {
            Logger::error("[RAM] FSR points to invalid address: " + std::to_string(fsr));
            throw std::out_of_range("FSR points to invalid address: " + std::to_string(fsr));
        }
        T value = (bank == Bank::BANK_0) ? bank0[fsr] : bank1[fsr];
        //Logger::info("[RAM] Indirect access returned value: " + std::to_string(value));
        return value;
    }

    T value = (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
    //Logger::info("[RAM] Direct access returned value: " + std::to_string(value));
    return value;
}

template <typename T>
void RamMemory<T>::set(Bank bank, int address, const T& value) {
    std::lock_guard<std::mutex> lock(mutex);
    Logger::info("[RAM] Setting value " + std::to_string(value) + " to bank " + 
                 std::to_string(static_cast<int>(bank)) + " address " + std::to_string(address));
    
    if (address < 0 || address >= bank0.size()) {
        Logger::error("[RAM] Invalid address access: " + std::to_string(address));
        throw std::out_of_range("Address isn't implemented");
    }
    if (address == 0x00) {
        Logger::info("indirect");
        // Special case: Direct initialization of INDF register (address 0)
        if (initializing && bank == Bank::BANK_0) {
            bank0[address] = value;
            bank1[address] = value;  // INDF is mirrored
            Logger::info("[RAM] Direct INDF initialization");
            initializing = false; 
            return;
        }else{
            Logger::info("[RAM] Indirect set through FSR");
            handleIndirectSet(bank, value);
            return;
        }

        
    }

    try {
        int oldValue = (bank == Bank::BANK_0) ? bank0[address] : bank1[address];
        bool shouldMirror = checkSFRMirroring(bank, address);
        
        if (bank == Bank::BANK_0) {
            bank0[address] = value;
            if (shouldMirror) {
                bank1[address] = value;
                Logger::info("[RAM] Mirrored value to bank 1");
            }
        } else {
            bank1[address] = value;
            if (shouldMirror) {
                bank0[address] = value;
                Logger::info("[RAM] Mirrored value to bank 0");
            }
        }
        
        firePropertyChange("ram", address, oldValue, value);
        //Logger::info("[RAM] Value set successfully");
    } catch (const std::exception& e) {
        Logger::error("[RAM] Failed to set value: " + std::string(e.what()));
        throw;
    }
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

template <typename T>
void RamMemory<T>::clear() {
    std::lock_guard<std::mutex> lock(mutex);
    std::fill(bank0.begin(), bank0.end(), T{});
    std::fill(bank1.begin(), bank1.end(), T{});
    Logger::info("[RAM] Memory cleared");
}


template <typename T>
void RamMemory<T>::firePropertyChange(const std::string& propertyName, int index, T oldValue, T newValue) {
    if (propertyChangeListeners.find(propertyName) != propertyChangeListeners.end()) {
        propertyChangeListeners[propertyName](index, oldValue, newValue);
    }
}

template <typename T>
bool RamMemory<T>::checkSFRMirroring(Bank bank, int address) const {
    for (const auto& sfr : SFR::entries()) {
        if (sfr.address == address && sfr.mapped) {
            return true;  // If any SFR at this address is mirrored, return true
        }
    }
    return false;
}

template <typename T>
void RamMemory<T>::handleIndirectSet(Bank bank, const T& value) {
    Logger::info("[RAM] Handling indirect set for bank " + std::to_string(static_cast<int>(bank)) + " with value " + std::to_string(value));
    // FSR is mirrored, so read from the same bank we're accessing
    int fsrValue = (bank == Bank::BANK_0) ? bank0[0x04] : bank1[0x04];
    Logger::info("[RAM] Indirect set through FSR: " + std::to_string(fsrValue));
    
    if (fsrValue == 0x00) {
        Logger::error("[RAM] Indirect addressing loop detected");
        throw std::logic_error("Indirect addressing loop detected");
    }
    if (fsrValue >= bank0.size()) {
        Logger::error("[RAM] FSR points to invalid address: " + std::to_string(fsrValue));
        throw std::out_of_range("FSR points to invalid address: " + std::to_string(fsrValue));
    }

    int oldValue = (bank == Bank::BANK_0) ? bank0[fsrValue] : bank1[fsrValue];
    
    // Check if the target address should be mirrored
    bool shouldMirror = checkSFRMirroring(bank, fsrValue);
    
    if (bank == Bank::BANK_0) {
        bank0[fsrValue] = value;
        if (shouldMirror) {
            bank1[fsrValue] = value;
            Logger::info("[RAM] Mirrored indirect value to bank 1");
        }
    } else {
        bank1[fsrValue] = value;
        if (shouldMirror) {
            bank0[fsrValue] = value;
            Logger::info("[RAM] Mirrored indirect value to bank 0");
        }
    }
    
    firePropertyChange("ram", fsrValue, oldValue, value);
    //Logger::info("[RAM] Indirect value set successfully");
}

template class RamMemory<uint8_t>;