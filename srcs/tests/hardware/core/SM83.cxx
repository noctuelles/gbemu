//
// Created by plouvel on 4/23/25.
//

#include "tests/hardware/core/SM83.hxx"

namespace Test
{
    void SM83::SetUp() override
    {
        this->cpu = std::make_unique<::SM83>(*address_space, [] {});
    }

    void SM83::TearDown() override
    {
        this->address_space.reset();
        this->cpu.reset();
    }

    void SM83::execute_instruction(std::initializer_list<uint8_t> instruction) const
    {
        for (const auto& byte : instruction)
        {
            address_space->write(cpu->PC, byte);
        }

        cpu->fetch_instruction();
        cpu->decode_execute_instruction();
    }
}  // namespace Test