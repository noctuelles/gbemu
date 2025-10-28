//
// Created by plouvel on 28/10/2025.
//

#include "ui/AddressSpaceMemoryEditor.hxx"

AddressSpaceMemoryEditor::AddressSpaceMemoryEditor()
{
    editor.ReadFn   = readFn;
    editor.WriteFn  = writeFn;
    editor.Cols     = 8;
    editor.UserData = this;
}

void AddressSpaceMemoryEditor::render()
{
    editor.DrawWindow("Address Space", addressSpace.data(), addressSpace.size());
}

ImU8 AddressSpaceMemoryEditor::readFn(const ImU8* data, size_t offset, void* user_data)
{
    (void) data;

    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};
    return thisPtr->addressSpace.at(offset);
}

void AddressSpaceMemoryEditor::writeFn(ImU8* mem, size_t off, ImU8 d, void* user_data)
{
    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};

    (void) thisPtr;
    (void) mem;
    (void) off;
    (void) d;
    (void) user_data;
}