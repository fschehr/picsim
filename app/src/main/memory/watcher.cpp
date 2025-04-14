#include <stdexcept>
#include <functional>

template <typename T>
class MemoryWatcher {
public:
    virtual ~MemoryWatcher() = default;

    // Add a property change listener
    virtual void addPropertyChangeListener(const std::function<void(int, T, T)>& listener) = 0;

    // Remove a property change listener
    virtual void removePropertyChangeListener(const std::function<void(int, T, T)>& listener) = 0;

    // Get a value from the memory at the specified address
    virtual T get(int address) const = 0;
};