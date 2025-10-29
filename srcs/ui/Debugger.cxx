//
// Created by plouvel on 28/10/2025.
//

#include "ui/Debugger.hxx"

#include <format>
#include <ranges>
#include <vector>

#include "imgui.h"
#include "ui/ForkAwesomeFont.hxx"

Debugger::Debugger(SM83& cpu) : cpu(cpu), instructions(10, {false, "$1000:", "LD A, A"}) {}

void Debugger::render()
{
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
        ImGui::Button(ICON_FK_PLAY);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Continue");
        }

        ImGui::SameLine();

        if (ImGui::Button(ICON_FK_ARROW_DOWN))
        {
            cpu.tick();
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
        const auto availableWidth{ImGui::GetContentRegionAvail().x};

        ImGui::BeginChild("##instructionList", ImVec2(availableWidth * 0.75f, 0), true);

        ImGui::SeparatorText("Instructions List");
        ImGui::Spacing();

        SM83::Disassembler disassembler{cpu.bus};

        auto list{disassembler.disassemble(cpu.PC, 100)};

        for (auto [i, instruction] : std::views::enumerate(list))
        {
            auto breakpointActive = false;

            ImGui::PushID(i);
            if (ImGui::InvisibleButton("##hoverBtn", ImVec2(18, 18)))
            {
                breakpointActive = !breakpointActive;
            }

            if (ImGui::IsItemHovered() || breakpointActive)
            {
                const auto pos{ImGui::GetItemRectMin()};
                const auto opacity{breakpointActive ? 1.0f : 0.5f};
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

            if (selectedIndex == i)
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

        ImGuiTextRegister("A", cpu.A);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("AF", cpu.AF(), true);

        ImGuiTextRegister("B", cpu.B);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("BC", cpu.BC(), true);

        ImGuiTextRegister("C", cpu.C);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("DE", cpu.DE(), true);

        ImGuiTextRegister("D", cpu.D);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("HL", cpu.HL(), true);

        ImGuiTextRegister("E", cpu.E);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("SP", cpu.SP, true);

        ImGuiTextRegister("H", cpu.H);
        ImGui::SameLine(0, 20);
        ImGuiTextRegister("PC", cpu.PC, true);

        ImGuiTextRegister("L", cpu.L);

        ImGui::Spacing();
        ImGui::SeparatorText("CPU Flags");
        ImGui::Spacing();

        const auto     spacing{ImGui::GetContentRegionAvail().x / 4.f};
        constexpr auto green{ImVec4(0, 1, 0, 1)};
        constexpr auto red{ImVec4(1, 0, 0, 1)};

        ImGui::TextColored(cpu.get_flag(SM83::Flags::Zero) ? green : red, "Z");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Zero flag");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(cpu.get_flag(SM83::Flags::Subtract) ? green : red, "N");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Substraction flag (BCD)");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(cpu.get_flag(SM83::Flags::HalfCarry) ? green : red, "H");
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Half Carry flag (BCD)");
        }
        ImGui::SameLine(0, spacing);
        ImGui::TextColored(cpu.get_flag(SM83::Flags::Carry) ? green : red, "C");
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

void Debugger::onInstructionFetched() {}

void Debugger::onInstructionExecuted()
{
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
