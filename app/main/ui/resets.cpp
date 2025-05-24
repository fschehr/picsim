#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "../logger.h"
#include "../simvm.h"

void resetStatusRegister(PicSimulatorVM &vm) {
    bool statusBits[8] = {false, false, false, true, true, false, false, false};
    uint8_t statusValue = 0;
    for (int j = 0; j < 8; ++j) {
        if (statusBits[j]) {
            statusValue |= (1 << j);
        }
    }
    vm.executor.setRamContent(RamMemory<uint8_t>::Bank::BANK_0, 0x03, statusValue);
}

void resetWRegister(PicSimulatorVM &vm) {
    vm.executor.setWorkingRegister(0x00);
}

void resetPortA(PicSimulatorVM &vm) {
    bool portABits[8] = {false, false, false, false, false, false, false, false};
    uint8_t portAValue = 0;
    for (int j = 0; j < 8; ++j) {
        if (portABits[j]) {
            portAValue |= (1 << j);
        }
    }
    vm.executor.setRamContent(RamMemory<uint8_t>::Bank::BANK_0, 0x05, portAValue);
}

void resetPortB(PicSimulatorVM &vm) {
    bool portBBits[8] = {false, false, false, false, false, false, false, false};
    uint8_t portBValue = 0;
    for (int j = 0; j < 8; ++j) {
        if (portBBits[j]) {
            portBValue |= (1 << j);
        }
    }
    vm.executor.setRamContent(RamMemory<uint8_t>::Bank::BANK_0, 0x06, portBValue);
}

void resetTrisA(PicSimulatorVM &vm) {
    bool trisABits[8] = {true, true, true, true, true, false, false, false};
    uint8_t trisAValue = 0;
    for (int j = 0; j < 8; ++j) {
        if (trisABits[j]) {
            trisAValue |= (1 << j);
        }
    }
    vm.executor.setRamContent(0x85, trisAValue);
}

void resetTrisB(PicSimulatorVM &vm) {
    bool trisBBits[8] = {true, true, true, true, true, true, true, true};
    uint8_t trisBValue = 0;
    for (int j = 0; j < 8; ++j) {
        if (trisBBits[j]) {
            trisBValue |= (1 << j);
        }
    }
    vm.executor.setRamContent(0x86, trisBValue);
}

/**
 * @brief Creates the Resets component.
 * 
 * @param vm The PicSimulatorVM instance.
 * 
 * @return ftxui::Component The Resets component.
 */
ftxui::Component Resets(
    PicSimulatorVM &vm
) {
    using namespace ftxui;
    
    auto button_tmr0 = Button("TMR0", [] {
        Logger::info("Resetting TMR0 register");
    });
    
    auto button_pcl = Button("PCL", [] {
        Logger::info("Resetting PCL register");
    });
    
    auto button_status = Button("STATUS", [&vm] {
        Logger::info("Resetting STATUS register");
        resetStatusRegister(vm);
    });
    
    auto button_fsr = Button("FSR", [] {
        Logger::info("Resetting FSR register");
    });
    
    auto button_porta = Button("PORTA", [&vm] {
        Logger::info("Resetting PORTA register");
        resetPortA(vm);
    });
    
    auto button_portb = Button("PORTB", [&vm] {
        Logger::info("Resetting PORTB register");
        resetPortB(vm);
    });
    
    auto button_eedata = Button("EEDATA", [] {
        Logger::info("Resetting EEDATA register");
    });
    
    auto button_eeadr = Button("EEADR", [] {
        Logger::info("Resetting EEADR register");
    });
    
    auto button_pclath = Button("PCLATH", [] {
        Logger::info("Resetting PCLATH register");
    });
    
    auto button_intcon = Button("INTCON", [] {
        Logger::info("Resetting INTCON register");
    });
    
    auto button_option = Button("OPTION", [] {
        Logger::info("Resetting OPTION register");
    });
    
    auto button_trisa = Button("TRISA", [&vm] {
        Logger::info("Resetting TRISA register");
        resetTrisA(vm);
    });
    
    auto button_trisb = Button("TRISB", [&vm] {
        Logger::info("Resetting TRISB register");
        resetTrisB(vm);
    });
    
    auto button_eecon1 = Button("EECON1", [] {
        Logger::info("Resetting EECON1 register");
    });
    
    auto button_eecon2 = Button("EECON2", [] {
        Logger::info("Resetting EECON2 register");
    });
    
    auto button_wreg = Button("W-REG", [&vm] {
        Logger::info("Resetting W register");
        resetWRegister(vm);
    });

    auto buttons_row = Container::Horizontal({
        button_tmr0,
        button_pcl,
        button_status,
        button_fsr,
        button_porta,
        button_portb,
        button_eedata,
        button_eeadr,
        button_pclath,
        button_intcon,
        button_option,
        button_trisa,
        button_trisb,
        button_eecon1,
        button_eecon2,
        button_wreg
    });
    
    auto resets_renderer = Renderer(buttons_row, [
        button_tmr0, button_pcl, button_status, button_fsr, 
        button_porta, button_portb, button_eedata, button_eeadr,
        button_pclath, button_intcon, button_option, button_trisa,
        button_trisb, button_eecon1, button_eecon2, button_wreg
    ] {
        return window(
            text(" Reset SFRs "),
            hbox({
                button_tmr0->Render() | xflex,
                button_pcl->Render() | xflex,
                button_status->Render() | xflex,
                button_fsr->Render() | xflex,
                button_porta->Render() | xflex,
                button_portb->Render() | xflex,
                button_eedata->Render() | xflex,
                button_eeadr->Render() | xflex,
                button_pclath->Render() | xflex,
                button_intcon->Render() | xflex,
                button_option->Render() | xflex,
                button_trisa->Render() | xflex,
                button_trisb->Render() | xflex,
                button_eecon1->Render() | xflex,
                button_eecon2->Render() | xflex,
                button_wreg->Render() | xflex
            }) | center
        );
    });

    return resets_renderer;
}