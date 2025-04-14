#include <functional>
#include <vector>
#include <cstdint>

class ExecutionWatcher {
public:
    virtual ~ExecutionWatcher() = default;

    /**
     * Returns the content of the working register.
     */
    virtual uint8_t getWorkingRegister() const = 0;

    /**
     * The instruction register contains the next executable instruction before its execution.
     *
     * @return Returns the content of the instruction register.
     */
    virtual uint16_t getInstructionRegister() const = 0;

    /**
     * Allows read-only access to the program counter (instruction pointer).
     *
     * @return Returns the current address the instruction pointer is pointing to.
     */
    virtual int getProgramCounter() const = 0;

    /**
     * Allows fetching the current state of the runtime counter in microseconds.
     *
     * @return Returns the current state.
     */
    virtual double getRuntimeCounter() const = 0;

    /**
     * Adds a change listener **only** for observing the executor's state.
     *
     * @param listener The listener that should be registered.
     */
    virtual void addPropertyChangeListener(const std::function<void()>& listener) = 0;

    /**
     * Removes a change listener **only** from the change support of the executor.
     *
     * @param listener The listener that should be removed.
     */
    virtual void removePropertyChangeListener(const std::function<void()>& listener) = 0;
};