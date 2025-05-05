#ifndef SIMVM_H
#define SIMVM_H

#include <vector>
#include <string>
#include <instruction.h>
class ConcreteInstruction{
    public:
        ConcreteInstruction(OperationCode opc, const std::string& arguments);
        ConcreteInstruction(Instruction inst);
        OperationCode getConcOpc();
        std::string getConcArguments(); 
    private:
        std::string arguments;
}
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
