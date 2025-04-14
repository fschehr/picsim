#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "instruction.cpp"
#include "memory/ram.cpp"
#include "memory/stack.cpp"
#include "memory/eeprom.cpp"
#include "memory/program.cpp"
#include "decode.cpp"

class ConcreteInstruction : public Instruction {
private:
    int opc;
    std::string arguments;

public:
    ConcreteInstruction(int opc, const std::string& arguments) : opc(opc), arguments(arguments) {}

    int getOpc() override {
        return opc;
    }

    std::string getArguments() override {
        return arguments;
    }
};

class InstructionExecution {
private:
    ProgramMemory& program;
    RamMemory& ram;
    StackMemory& stack;
    EepromMemory& eeprom;

public:
    InstructionExecution(ProgramMemory& program, RamMemory& ram, StackMemory& stack, EepromMemory& eeprom)
        : program(program), ram(ram), stack(stack), eeprom(eeprom) {}

    int execute() {
        // Implement the execution logic here
        // For demonstration purposes, return a dummy value
        return 0;
    }

    void reset() {
        // Implement the reset logic here
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
    Decoder decoder; // Use the Decoder class
            Instruction* instruction = new ConcreteInstruction(prog[i], ""); // Replace with a concrete implementation

    bool running = false;
    bool loaded = false;

public:
    PicSimulatorVM() : ram(ramSize), program(programMemorySize), programMemory(programMemorySize), stack(stackSize), eeprom(eepromSize), executor(program, ram, stack, eeprom) {}

    void initialize(const std::vector<short>& prog) {
        programDecode(prog);
        load(prog);
        while (running) {
            execute();
        }
    }

    void programDecode(const std::vector<short>& prog) {
        for (size_t i = 0; i < prog.size(); i++) {
            Instruction* instruction = new Instruction(decoder.decode(prog[i]));
            programMemory[i] = instruction;
        }
        for (size_t i = 0; i < programMemory.size(); i++) {
            if (programMemory[i] != nullptr) {
                std::cout << "Instruction " << static_cast<int>(programMemory[i]->getOpc()) << " with arguments " << programMemory[i]->getArguments() << std::endl;
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

