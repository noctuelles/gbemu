//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <cstdint>
#include <string>
#include <vector>

#include "hardware/core/SM83.hxx"

class Debugger
{
  public:
    explicit Debugger(SM83 &cpu);

    void render();
    void setDisabled(bool _disabled);

  private:
    void ImGuiTextRegister(const std::string& regName, uint16_t value, bool sixteenBitsRegister = false) const;

    SM83 &cpu;
    int  selectedIndex{};
    bool showValueAsHexadecimal{true};
    bool showValueAsDecimal{false};
    bool disabled{false};

    std::vector<std::tuple<bool, const char*, std::string>> instructions;
};

#endif  // GBEMU_DEBUGGER_HXX
