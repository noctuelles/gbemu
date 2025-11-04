//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_MEMORYEDITOR_HXX
#define GBEMU_MEMORYEDITOR_HXX

#include <optional>
#include <span>

#include "Emulator.hxx"
#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

class AddressSpaceMemoryEditor
{
  public:
    explicit AddressSpaceMemoryEditor();

    std::optional<Emulator::Command> render();

    void setAddressSpace(std::array<uint8_t, 0x10000> addrSpace);

  private:
    MemoryEditor                     editor{};
    std::optional<Emulator::Command> cmd{};

    static ImU8 readFn(const ImU8* mem, size_t offset, void* user_data);
    static void writeFn(ImU8* mem, size_t offset, ImU8 d, void* user_data);

    std::array<uint8_t, 0x10000> addressSpace{};
};

#endif  // GBEMU_MEMORYEDITOR_HXX
