#include "simvm.h"
#include "instruction.h"
#include "memory/ram.h"
#include "memory/stack.h"
#include "memory/eeprom.h"
#include "memory/program.h"
#include "decode.h"
#include "exec/instructionex.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

class ConcreteInstruction : public Instruction {
private:
    int opc;
    std::string arguments;

public:
    ConcreteInstruction(int opc, const std::string& arguments) : opc(opc), arguments(arguments) {}

    OperationCode getOpc() override {
        return opc;
    }

    std::string getArguments() override {
        return arguments;
    }
};

class PicSimulatorVM {
private:
    Logger logger;
    int ramSize = 128;
    int programMemorySize = 1024;
    int stackSize = 8;
    int eepromSize = 64;

    RamMemory ram;
    ProgramMemory program;
    std::vector<Instruction*> programMemory; 
    StackMemory stack;
    EepromMemory eeprom;
    Decoder decoder;
    InstructionExecution executor;

    bool running = false;
    bool loaded = false;

public:
    PicSimulatorVM()
        : logger("PicSimulatorVM"), ram(ramSize), program(programMemorySize), programMemory(programMemorySize),
          stack(stackSize), eeprom(eepromSize), executor(program, ram, stack, eeprom) {}

    ~PicSimulatorVM() {
        for (Instruction* instruction : programMemory) {
            delete instruction;
        }
    }

    void initialize(const std::vector<short>& prog) {
        programDecode(prog);
        load(prog);
        while (running) {
            execute();
        }
    }

    void programDecode(const std::vector<short>& prog) {
        if (prog.size() > programMemory.size()) {
            throw std::runtime_error("Program size exceeds program memory capacity");
        }
        for (size_t i = 0; i < prog.size(); i++) {
            Instruction* instruction = new ConcreteInstruction(decoder.decode(prog[i]), ""); //what is the next step of the operation? https://www.youtube.com/watch?v=UFM2tJzi2NI&pp=ygUmd2hhdCBpcyB0aGUgbmV4dCBzdGVwIG9mIHRoZSBvcGVyYXRpb24%3D
            programMemory[i] = instruction;
        }
        for (size_t i = 0; i < programMemory.size(); i++) {
            if (programMemory[i] != nullptr) {
                std::cout << "Instruction " << programMemory[i]->getOpc() << " with arguments " << programMemory[i]->getArguments() << std::endl;
            }
        }
    }

    void stop() {
        running = false;
    }

    void load(const std::vector<short>& file) {
        stop();

        for (size_t address = 0; address < file.size(); address++) {
            program.set(address, file[address]);
        }

        loaded = true;
        executor.reset();
    }

    int execute() {
        if (!loaded) {
            throw std::runtime_error("No executable program loaded");
        } else {
            if (!running) {
                running = true;
                executor.reset();
            }
            return executor.execute();
        }
    }
};
