//
// Created by plouvel on 12/30/24.
//
#pragma once

#include <gtest/gtest.h>

#include <Bus.hxx>
#include <CPU.hxx>
#include <random>

class CPUTesting : public ::testing::Test
{
  protected:
    std::unique_ptr<Bus> bus{};
    std::unique_ptr<CPU> cpu{};

    void SetUp() override
    {
        this->bus = std::make_unique<Bus>();
        this->cpu = std::make_unique<CPU>(*bus);
    }

    void TearDown() override
    {
        this->bus.reset();
        this->cpu.reset();
    }

  public:
    /**
     * @brief Execute an instruction
     * @param instruction Instruction to be executed by the CPU.
     * @param ticks Number of ticks (T-cycle).
     */
    void execute_instruction(const std::initializer_list<uint8_t> instruction, size_t ticks) const
    {
        this->bus->write(this->cpu->reg.u16.PC, instruction);
        while (ticks--)
        {
            this->cpu->tick();
        };
    }

    static auto generate_address(std::pair<uint16_t, uint16_t>&& range = std::make_pair(0, Bus::RAM_SIZE - 1))
    {
        std::random_device                      rd{};
        std::mt19937                            gen{rd()};
        std::uniform_int_distribution<uint16_t> distrib{range.first, range.second};

        return distrib(gen);
    }

    [[nodiscard]] static auto generate_byte()
    {
        std::random_device                     rd{};
        std::mt19937                           gen{rd()};
        std::uniform_int_distribution<uint8_t> distrib{0, UINT8_MAX};

        return distrib(gen);
    }
};

struct TestInstructionParam
{
    uint8_t opcode;
};
