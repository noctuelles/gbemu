//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_MEMORYEDITOR_HXX
#define GBEMU_MEMORYEDITOR_HXX

#include <array>

#include "hardware/Addressable.hxx"
#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"

class AddressSpaceMemoryEditor
{
  public:
    explicit AddressSpaceMemoryEditor(Addressable& bus);

    void render();

  private:
    Addressable& bus;
    MemoryEditor editor{};

    static ImU8 readFn(const ImU8* mem, size_t offset, void* user_data);
    static void writeFn(ImU8* mem, size_t offset, ImU8 d, void* user_data);

    std::array<uint8_t, 0x10000> addressSpace{};
};

#endif  // GBEMU_MEMORYEDITOR_HXX
