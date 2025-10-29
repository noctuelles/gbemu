//
// Created by plouvel on 12/30/24.
//
#pragma once

#include <gtest/gtest.h>

#include <memory>

#include "hardware/Bus.hxx"
#include "hardware/core/SM83.hxx"
#include "tests/hardware/FakeRAM.hxx"

namespace Test
{
    class SM83 : public ::testing::Test
    {
      protected:
        std::unique_ptr<::SM83>  cpu{};
        std::unique_ptr<FakeRAM> address_space{};

        void SetUp() override;
        void TearDown() override;

      public:
        /**
         * @brief Execute an instruction
         * @param instruction Instruction to be executed by the CPU.
         */
        void executeInstruction(std::initializer_list<uint8_t> instruction) const;
    };
}  // namespace Test

struct TestInstructionParam
{
    uint8_t opcode;
};
