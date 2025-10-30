//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <cstdint>
#include <set>
#include <string>

#include "Common.hxx"
#include "IDebugger.hxx"
#include "hardware/core/SM83.hxx"

class Debugger final : public IDebugger<SM83::View>
{
  public:
    explicit Debugger(Addressable& bus);

    void render();
    void setDisabled(bool _disabled);

    [[nodiscard]] EmulationState getEmulationState() const;

    void onCpuInitialization(SM83::View view) override;
    void onCpuInstructionFetched(SM83::View view) override;
    void onCpuInstructionExecuted(SM83::View view) override;

  private:
    void ImGuiTextRegister(const std::string& regName, uint16_t value, bool sixteenBitsRegister = false) const;

    Addressable&       bus;
    SM83::Disassembler disassembler;
    SM83::View         cpuView{};
    EmulationState     emulationState{EmulationState::HALTED};

    int  selectedIndex{};
    bool showValueAsHexadecimal{true};
    bool showValueAsDecimal{false};
    bool disabled{false};

    std::set<uint16_t> breakpoints;
};

#endif  // GBEMU_DEBUGGER_HXX
