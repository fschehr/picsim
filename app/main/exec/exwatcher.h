#ifndef EXWATCHER_H
#define EXWATCHER_H

#include <functional>
#include <cstdint>

class ExecutionWatcher {
public:
    virtual ~ExecutionWatcher() = default;

    virtual uint8_t getWorkingRegister() const = 0;
    virtual uint16_t getInstructionRegister() const = 0;
    virtual int getProgramCounter() const = 0;
    virtual double getRuntimeCounter() const = 0;

    virtual void addPropertyChangeListener(const std::function<void()>& listener) = 0;
    virtual void removePropertyChangeListener(const std::function<void()>& listener) = 0;
};

#endif // EXWATCHER_H
