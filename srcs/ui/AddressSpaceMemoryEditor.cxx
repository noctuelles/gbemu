//
// Created by plouvel on 28/10/2025.
//

#include "ui/AddressSpaceMemoryEditor.hxx"

#include <iostream>
#include <ostream>

AddressSpaceMemoryEditor::AddressSpaceMemoryEditor()
{
    editor.ReadFn   = readFn;
    editor.WriteFn  = writeFn;
    editor.Cols     = 8;
    editor.UserData = this;
}

std::optional<Emulator::Command> AddressSpaceMemoryEditor::render()
{
    cmd = std::nullopt;

    if (addressSpace.has_value())
    {
        editor.DrawWindow("Address Space", nullptr, addressSpace.value().size());
    }

    return cmd;
}

void AddressSpaceMemoryEditor::setAddressSpace(std::span<const uint8_t, 65536> addressSpace)
{
    this->addressSpace = addressSpace;
}

ImU8 AddressSpaceMemoryEditor::readFn(const ImU8* mem, const size_t offset, void* user_data)
{
    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};
    (void) mem;

    return thisPtr->addressSpace.value()[offset];
}

void AddressSpaceMemoryEditor::writeFn(ImU8* mem, const size_t offset, const ImU8 d, void* user_data)
{
    auto thisPtr{static_cast<AddressSpaceMemoryEditor*>(user_data)};
    (void) mem;

    thisPtr->cmd =
        Emulator::Command(Emulator::Command::Type::WriteAddressSpace, Emulator::Command::WriteAddressSpace(offset, d));
}