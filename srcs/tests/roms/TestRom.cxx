#include "../../../includes/tests/roms/TestRom.hxx"

#include <fstream>

TestRom::Component::Component()
    : _state(), bus(_state), cpu(_state, bus, timer, ppu), echoRam(workRam), timer(bus), ppu(bus)
{
    bus.attach(timer);
    bus.attach(ppu);
    bus.attach(cpu);
    bus.attach(echoRam);
    bus.attach(workRam);
    bus.attach(fakeRam);
}

void TestRom::SetUp()
{
    _component = std::make_unique<Component>();
}

void TestRom::TearDown()
{
    _component.reset();
}

void TestRom::loadROM(const std::string& romPath) const
{
    std::ifstream               input{romPath, std::ios::binary};
    std::array<uint8_t, 0x8000> buffer{};

    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    input.close();

    for (const auto [i, byte] : std::views::enumerate(buffer))
    {
        _component->fakeRam.write(i, byte);
    }
}