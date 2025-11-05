//
// Created by plouvel on 28/10/2025.
//

#include "ui/Debugger.hxx"

#include <format>
#include <ranges>
#include <utility>
#include <vector>

#include "imgui.h"
#include "ui/ForkAwesomeFont.hxx"

Debugger::Debugger()
{
    memEditor.Cols     = 8;
    memEditor.UserData = this;
}

std::optional<Emulator::Command> Debugger::render()
{
    std::optional<Emulator::Command> ret{std::nullopt};

    ImGui::Begin("Debugger", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::BeginDisabled(disabled);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Display"))
        {
            if (ImGui::MenuItem("Show value as hexadecimal", nullptr, &showValueAsHexadecimal))
            {
                showValueAsDecimal = false;
            }

            if (ImGui::MenuItem("Show value as decimal", nullptr, &showValueAsDecimal))
            {
                showValueAsHexadecimal = false;
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::BeginGroup();
    {
        if (ImGui::Button(ICON_FK_PLAY))
        {
            ret = Emulator::Command(Emulator::Command::Type::Continue, {});
            setDisabled(true);
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Continue");
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_ARROW_DOWN))
        {
            ret = Emulator::Command(Emulator::Command::Type::Step, {});
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Step in");
        }

        ImGui::SameLine();

        ImGui::Button(ICON_FK_ARROW_RIGHT);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Step over");
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FK_LIST_OL))
        {
            setScrollToCurrentInstruction();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Go to program counter");
        }
    }
    ImGui::EndGroup();

    auto optionBarHeight{ImGui::GetFontSize() * 2.f + ImGui::GetStyle().FramePadding.y * 2.f};

    auto sizeRegisters{ImGui::CalcTextSize("X: $00XX: $0000")};
    auto constexpr padRegister{30.f};
    auto const registersWidth{sizeRegisters.x + ImGui::GetStyle().FramePadding.x * 2.f + padRegister};

    auto availableSpace{ImGui::GetContentRegionAvail()};

    ImGui::BeginChild("##instructionListArea",
                      ImVec2(availableSpace.x - registersWidth, availableSpace.y - optionBarHeight),
                      ImGuiChildFlags_None);
    {
        ImGui::SeparatorText("Instructions List");
        ImGui::Spacing();

        ImGui::BeginChild("##instructionList", ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_None);
        {
            for (auto [i, instruction] : std::views::enumerate(disassembledInstructions))
            {
                const auto glyphSize{ImGui::CalcTextSize("A")};
                const auto availableInstructionSpace{ImGui::GetContentRegionAvail()};
                const auto isAddressInBreakpoints{breakpoints.contains(instruction.first.first)};
                const auto nbrCharactersPerLine{availableInstructionSpace.x / glyphSize.x};

                ImGui::PushID(i);
                if (ImGui::InvisibleButton("##hoverBtn", ImVec2(18, 18)))
                {
                    if (isAddressInBreakpoints)
                    {
                        breakpoints.erase(instruction.first.first);

                        ret = Emulator::Command(Emulator::Command::Type::RemoveBreakpoint,
                                                Emulator::Command::Breakpoint{instruction.first.first});
                    }
                    else
                    {
                        breakpoints.insert(instruction.first.first);

                        ret = Emulator::Command(Emulator::Command::Type::SetBreakpoint,
                                                Emulator::Command::Breakpoint{instruction.first.first});
                    }
                }

                if (ImGui::IsItemHovered() || isAddressInBreakpoints)
                {
                    const auto pos{ImGui::GetItemRectMin()};
                    const auto opacity{isAddressInBreakpoints ? 1.f : 0.5f};
                    auto       drawList{ImGui::GetWindowDrawList()};

                    drawList->AddText(ImVec2(pos.x + 4.5f, pos.y + 2.5f), IM_COL32(255, 0, 0, 255 * opacity),
                                      ICON_FK_CIRCLE, nullptr);
                }

                std::string byte_dump{};
                for (const auto byte : instruction.first.second)
                {
                    byte_dump += std::string{std::format("{:02X} ", byte)};
                }

                ImGui::SameLine();

                const int padInstructionMnemonic{static_cast<int>(nbrCharactersPerLine - 30)};

                ImGui::Text("$%04X%20s%*s", instruction.first.first, byte_dump.c_str(), padInstructionMnemonic,
                            instruction.second.c_str());

                if (const auto& registers{cpuView.registers}; registers.PC == instruction.first.first)
                {
                    auto*      drawList = ImGui::GetWindowDrawList();
                    const auto style    = ImGui::GetStyle();
                    const auto color    = ImGui::GetStyleColorVec4(ImGuiCol_Header);

                    auto pMin{ImVec2(ImGui::GetItemRectMin().x - style.FramePadding.x,
                                     ImGui::GetItemRectMin().y - style.FramePadding.y)};
                    auto pMax{ImVec2(ImGui::GetItemRectMax().x + style.FramePadding.x,
                                     ImGui::GetItemRectMax().y + style.FramePadding.y)};

                    drawList->AddRectFilled(pMin, pMax,
                                            IM_COL32(color.x * 255.f, color.y * 255.f, color.z * 255.f,
                                                     color.w * 255.f));

                    if (scrollToCurrentInstruction)
                    {
                        ImGui::SetScrollHereY();
                        scrollToCurrentInstruction = false;
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##registers", ImVec2(registersWidth, availableSpace.y - optionBarHeight), ImGuiChildFlags_None,
                      ImGuiWindowFlags_None);
    {
        const auto& registers{cpuView.registers};

        ImGui::SeparatorText("CPU Registers");
        ImGui::Spacing();

        ImGuiTextRegister("A", registers.A);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("AF", registers.AF, true);

        ImGuiTextRegister("B", registers.B);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("BC", registers.BC, true);

        ImGuiTextRegister("C", registers.C);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("DE", registers.DE, true);

        ImGuiTextRegister("D", registers.D);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("HL", registers.HL, true);

        ImGuiTextRegister("E", registers.E);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("SP", registers.SP, true);

        ImGuiTextRegister("H", registers.H);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("PC", registers.PC, true);

        ImGuiTextRegister("L", registers.L);

        ImGui::Spacing();
        ImGui::SeparatorText("CPU Flags");
        ImGui::Spacing();

        const auto     spacing{ImGui::GetContentRegionAvail().x / 4.f};
        constexpr auto green{ImVec4(0, 1, 0, 1)};
        constexpr auto red{ImVec4(1, 0, 0, 1)};
        auto getFlag = [&registers](const SM83::Flags flag) { return registers.F & std::to_underlying(flag); };

        ImGui::TextColored(getFlag(SM83::Flags::Zero) ? green : red, "Z");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Zero flag");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(getFlag(SM83::Flags::Subtract) ? green : red, "N");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Substraction flag (BCD)");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(getFlag(SM83::Flags::HalfCarry) ? green : red, "H");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Half Carry flag (BCD)");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(getFlag(SM83::Flags::Carry) ? green : red, "C");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Carry flag");
        }
    }
    ImGui::EndChild();

    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::BeginChild("##optionBar", ImVec2(0, optionBarHeight), ImGuiChildFlags_None, ImGuiWindowFlags_None);
    {
        const auto glyphSize{ImGui::CalcTextSize("F")};

        ImGui::SetNextItemWidth(5 * glyphSize.x + style.FramePadding.x * 2.0f);
        if (ImGui::InputText("Address", disassemblyStartAddressStr.data(), disassemblyStartAddressStr.size(),
                             ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            std::from_chars(disassemblyStartAddressStr.data(),
                            disassemblyStartAddressStr.data() + disassemblyStartAddressStr.size(),
                            disassemblyStartAddressValue, 16);
        }
        ImGui::SameLine(0, 15);
        ImGui::SetNextItemWidth(9 * glyphSize.x + style.FramePadding.x * 2.0f);
        if (ImGui::InputInt("Nbr", &nbrOfInstructionsToDisassemble, 1, 100))
        {
            if (nbrOfInstructionsToDisassemble < 1)
            {
                nbrOfInstructionsToDisassemble = 1;
            }
            else if (nbrOfInstructionsToDisassemble > 256)
            {
                nbrOfInstructionsToDisassemble = 256;
            }
        }
    }
    ImGui::EndChild();

    ImGui::EndDisabled();
    ImGui::End();

    return ret;
}
void Debugger::setDisabled(bool _disabled)
{
    this->disabled = _disabled;
}

void Debugger::setCpuView(SM83::View view)
{
    cpuView = view;
}

void Debugger::setAddressSpace(const std::array<uint8_t, 0x10000>& addressSpace)
{
    const SM83::Disassembler disassembler{addressSpace};
    disassembledInstructions = disassembler.disassemble(disassemblyStartAddressValue, nbrOfInstructionsToDisassemble);
}

void Debugger::setScrollToCurrentInstruction()
{
    scrollToCurrentInstruction = true;
}

void Debugger::ImGuiTextRegister(const std::string& regName, const uint16_t value, bool sixteenBitsRegister) const
{
    std::string formattedValueHexadecimal{};
    std::string formattedValueDecimal{};
    std::string formattedValueBinary{};

    if (sixteenBitsRegister)
    {
        formattedValueHexadecimal = std::format("${:04X}", value);
        formattedValueBinary      = std::format("{:b}", value);
    }
    else
    {
        formattedValueHexadecimal = std::format("${:02X}", value);
        formattedValueBinary      = std::format("{:b}", value);
    }

    formattedValueDecimal = std::format("{}", value);

    ImGui::Text("%s: %s", regName.c_str(),
                showValueAsHexadecimal ? formattedValueHexadecimal.c_str() : formattedValueDecimal.c_str());
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("Decimal : %s", formattedValueDecimal.c_str());
        ImGui::Text("Hexadecimal : %s", formattedValueHexadecimal.c_str());
        ImGui::Text("Binary : %s", formattedValueBinary.c_str());
        ImGui::EndTooltip();
    }
}
