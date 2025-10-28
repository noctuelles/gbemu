//
// Created by plouvel on 28/10/2025.
//

#ifndef GBEMU_MEMORYEDITOR_HXX
#define GBEMU_MEMORYEDITOR_HXX

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include <array>

class AddressSpaceMemoryEditor
{
  public:
    AddressSpaceMemoryEditor();

    void render();

  private:
    MemoryEditor editor{};

    static ImU8 readFn(const ImU8* data, size_t offset, void* user_data);
    static void writeFn(ImU8* mem, size_t off, ImU8 d, void* user_data);

    std::array<uint8_t, 0x10000> addressSpace{};
};

#endif  // GBEMU_MEMORYEDITOR_HXX
