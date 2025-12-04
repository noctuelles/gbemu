//
// Created by plouvel on 12/4/25.
//
#include "tests/DummyComponent.hxx"

uint8_t DummyComponent::read(uint16_t address) const
{
    (void) address;
    return 0;
}

void DummyComponent::write(uint16_t address, uint8_t value)
{
    (void) address;
    (void) value;
}

void DummyComponent::tick(size_t machineCycle)
{
    (void) machineCycle;
}

IAddressable::AddressableRange DummyComponent::getAddressableRange() const noexcept
{
    return {};
}
