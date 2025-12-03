//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <qfile.h>

#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(std::optional<QString> bootRomPath, QObject* parent) : QObject(parent)
{
    if (!bootRomPath.has_value())
    {
        _components.bus.setPostBootRomRegisters();
        _components.cpu.setPostBootRomRegisters();
        _components.ppu.setPostBootRomRegisters();
        return;
    }

    QFile                      file{bootRomPath.value()};
    std::array<uint8_t, 0x100> bootRom{};

    if (!file.open(QIODevice::ReadOnly))
    {
        throw std::system_error();
    }

    if (static_cast<size_t>(file.size()) > bootRom.size())
    {
        throw std::runtime_error("Boot ROM size is too large (max 256 bytes)");
    }

    if (const auto readBytes{file.read(reinterpret_cast<char*>(bootRom.data()), bootRom.size())};
        readBytes != bootRom.size())
    {
        throw std::system_error{};
    }

    _components.bus.loadBootRom(bootRom);
}

void Emulator::startEmulation(const QString& path)
{
    _components.cartridge.load(path.toStdString());
    runFrame();
}

void Emulator::onKeyPressed(const Key key)
{
    _components.joypad.press(key);
}

void Emulator::onKeyReleased(const Key key)
{
    _components.joypad.release(key);
}

void Emulator::runFrame()
{
    using namespace std::chrono_literals;

    constexpr auto frameDuration{16740000ns};

    try
    {
        /* Run the emulation for a whole frame. A frame = 70,224 dots = 17,556 machine cycles. */
        _components.cpu.tick(17556);

        const auto now{std::chrono::steady_clock::now()};

        if (const auto diff{now - _lastUpdate}; diff < frameDuration)
        {
            std::this_thread::sleep_for(frameDuration - diff);
        }

        emit frameReady(_components.ppu.getFramebuffer());

        _lastUpdate = now;
    }
    catch (const std::exception& e)
    {
        emit emulationFatalError(e.what());
    }
}

Emulator::Components::Components()
    : _state(), bus(_state), timer(bus), ppu(bus), cpu(_state, bus, timer, ppu), echoRam(workRam)
{
    bus.attach(cartridge);
    bus.attach(timer);
    bus.attach(ppu);
    bus.attach(cpu);
    bus.attach(echoRam);
    bus.attach(joypad);
    bus.attach(workRam);
    bus.attach(fakeRam);
}