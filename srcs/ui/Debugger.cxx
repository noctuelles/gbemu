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

Debugger::Debugger(Addressable& bus) : bus(bus), disassembler(bus) {}

void Debugger::render()
{
    if (emulationState == EmulationState::SINGLE_CPU_TICK)
    {
        emulationState = EmulationState::HALTED;
    }

    ImGui::Begin("Debugger", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
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
            emulationState = EmulationState::NORMAL;
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Continue");
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_ARROW_DOWN))
        {
            emulationState = EmulationState::SINGLE_CPU_TICK;
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
    }
    ImGui::EndGroup();

    {
        auto availableWidth{ImGui::GetContentRegionAvail().x};

        ImGui::BeginChild("##instructionList", ImVec2(availableWidth * 0.75f, 0), true);

        ImGui::SeparatorText("Instructions List");
        ImGui::Spacing();

        for (auto list{disassembler.disassemble(0, 256)};
             auto [i, instruction] : std::views::enumerate(list))
        {
            const auto isAddressInBreakpoints{breakpoints.contains(instruction.first.first)};

            ImGui::PushID(i);
            if (ImGui::InvisibleButton("##hoverBtn", ImVec2(18, 18)))
            {
                if (isAddressInBreakpoints)
                {
                    breakpoints.erase(instruction.first.first);
                }
                else
                {
                    breakpoints.insert(instruction.first.first);
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
            ImGui::Text("$%04X %15s %30s", instruction.first.first, byte_dump.c_str(), instruction.second.c_str());

            if (cpuView.registers.PC == instruction.first.first)
            {
                auto       drawList = ImGui::GetWindowDrawList();
                const auto style    = ImGui::GetStyle();
                auto       color    = ImGui::GetStyleColorVec4(ImGuiCol_Header);

                auto pMin{ImVec2(ImGui::GetItemRectMin().x - style.FramePadding.x,
                                 ImGui::GetItemRectMin().y - style.FramePadding.y)};
                auto pMax{ImVec2(ImGui::GetItemRectMax().x + style.FramePadding.x,
                                 ImGui::GetItemRectMax().y + style.FramePadding.y)};

                drawList->AddRectFilled(pMin, pMax,
                                        IM_COL32(color.x * 255.f, color.y * 255.f, color.z * 255.f, color.w * 255.f));

                ImGui::SetScrollHereY();
            }

            ImGui::PopID();
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    ImGui::BeginChild("##registers", ImVec2(0, 0), ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    {
        ImGui::SeparatorText("CPU Registers");
        ImGui::Spacing();

        ImGuiTextRegister("A", cpuView.registers.A);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("AF", cpuView.registers.AF, true);

        ImGuiTextRegister("B", cpuView.registers.B);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("BC", cpuView.registers.BC, true);

        ImGuiTextRegister("C", cpuView.registers.C);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("DE", cpuView.registers.DE, true);

        ImGuiTextRegister("D", cpuView.registers.D);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("HL", cpuView.registers.HL, true);

        ImGuiTextRegister("E", cpuView.registers.E);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("SP", cpuView.registers.SP, true);

        ImGuiTextRegister("H", cpuView.registers.H);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("PC", cpuView.registers.PC, true);

        ImGuiTextRegister("L", cpuView.registers.L);

        ImGui::Spacing();
        ImGui::SeparatorText("CPU Flags");
        ImGui::Spacing();

        const auto     spacing{ImGui::GetContentRegionAvail().x / 4.f};
        constexpr auto green{ImVec4(0, 1, 0, 1)};
        constexpr auto red{ImVec4(1, 0, 0, 1)};
        auto getFlag = [this](const SM83::Flags flag) { return cpuView.registers.F & std::to_underlying(flag); };

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

    ImGui::EndDisabled();
    ImGui::End();
}
void Debugger::setDisabled(bool _disabled)
{
    this->disabled = _disabled;
}

EmulationState Debugger::getEmulationState() const
{
    return emulationState;
}

void Debugger::onCpuInitialization(SM83::View view)
{
    cpuView = view;
}

void Debugger::onCpuInstructionFetched(SM83::View view)
{
    (void) view;
}

void Debugger::onCpuInstructionExecuted(SM83::View view)
{
    cpuView = view;

    if (breakpoints.contains(cpuView.registers.PC))
    {
        emulationState = EmulationState::HALTED;
    }
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
