//
// Created by plouvel on 28/10/2025.
//

#include "ui/AddressSpaceMemoryEditor.hxx"

#include <iostream>
#include <ostream>

AddressSpaceMemoryEditor::AddressSpaceMemoryEditor(Addressable& bus) : bus(bus)
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

ImU8 AddressSpaceMemoryEditor::readFn(const ImU8* mem, const size_t offset, void* user_data)
{
    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};
    (void) mem;

    return thisPtr->bus.read(offset);
}

void AddressSpaceMemoryEditor::writeFn(ImU8* mem, const size_t offset, const ImU8 d, void* user_data)
{
    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};
    (void) mem;

    thisPtr->bus.write(offset, d);
}