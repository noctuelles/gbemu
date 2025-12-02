//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(QObject* parent) : QObject(parent), _components(nullptr) {}

void Emulator::loadRom(const QString& path)
{
    _components = std::make_unique<Components>();
    _components->cartridge.load(path.toStdString());
    runFrame();
}

void Emulator::onKeyPressed(const Key key) const
{
    _components->joypad.press(key);
}

void Emulator::onKeyReleased(const Key key) const
{
    _components->joypad.release(key);
}

void Emulator::runFrame()
{
    using namespace std::chrono_literals;

    constexpr auto frameDuration{16740000ns};

    try
    {
        /* Run the emulation for a whole frame. A frame = 70,224 dots = 17,556 machine cycles. */
        _components->cpu.tick(17556);

        const auto now{std::chrono::steady_clock::now()};

        if (const auto diff{now - _lastUpdate}; diff < frameDuration)
        {
            std::this_thread::sleep_for(frameDuration - diff);
        }

        emit frameReady(_components->ppu.getFramebuffer());

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