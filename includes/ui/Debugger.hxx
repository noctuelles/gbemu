//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <string>

#include "Emulator.hxx"
#include "hardware/core/SM83.hxx"
#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

class Debugger
{
  public:
    Debugger();

    std::optional<Emulator::Command> render();
    void                             setDisabled(bool _disabled);

    void setCpuView(SM83::View view);
    void setAddressSpace(const std::array<uint8_t, 0x10000>& addressSpace);
    void setScrollToCurrentInstruction();

  private:
    void ImGuiTextRegister(const std::string& regName, uint16_t value, bool sixteenBitsRegister = false) const;

    SM83::View                                   cpuView{};
    SM83::Disassembler::DisassembledInstructions disassembledInstructions{};

    uint16_t            disassemblyStartAddressValue{0x0000};
    std::array<char, 5> disassemblyStartAddressStr{"0000"};
    int                 nbrOfInstructionsToDisassemble{256};

    MemoryEditor                     memEditor{};

    int  selectedIndex{};
    bool showValueAsHexadecimal{true};
    bool showValueAsDecimal{false};
    bool disabled{false};

    bool scrollToCurrentInstruction{false};

    std::set<uint16_t> breakpoints;
};

#endif  // GBEMU_DEBUGGER_HXX
