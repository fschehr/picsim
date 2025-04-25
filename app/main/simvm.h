#ifndef SIMVM_H
#define SIMVM_H

#include <vector>
#include <string>

class PicSimulatorVM {
public:
    PicSimulatorVM();
    ~PicSimulatorVM();
    void initialize(const std::vector<short>& prog);
    void stop();
    void load(const std::vector<short>& file);
    int execute();
};

#endif // SIMVM_H
