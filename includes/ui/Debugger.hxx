//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <string>

#include "Emulator.hxx"
#include "hardware/core/SM83.hxx"

class Debugger
{
  public:
    Debugger();

    std::optional<Emulator::Command> render();
    void                             setDisabled(bool _disabled);

    void setCpuView(SM83::View view);
    void setAddressSpace(std::span<const uint8_t, 0x10000> addressSpace);

  private:
    void ImGuiTextRegister(const std::string& regName, uint16_t value, bool sixteenBitsRegister = false) const;

    std::optional<SM83::View>                        cpuView{};
    std::optional<std::span<const uint8_t, 0x10000>> addressSpace{};

    int  selectedIndex{};
    bool showValueAsHexadecimal{true};
    bool showValueAsDecimal{false};
    bool disabled{false};

    std::set<uint16_t> breakpoints;
};

#endif  // GBEMU_DEBUGGER_HXX
