//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <qfile.h>

#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(const std::optional<QString>& bootRomPath, QObject* parent)
    : QObject(parent), _renderer(new QtRenderer(this)), _components(*_renderer)
{
    if (!bootRomPath.has_value())
    {
        _components.bus.setPostBootRomRegisters();
        _components.cpu.setPostBootRomRegisters();
        _components.ppu.setPostBootRomRegisters();
        return;
    }

    connect(_renderer, &QtRenderer::onRender, this, &Emulator::onRender, Qt::DirectConnection);

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
    try
    {
        _components.cartridge.load(path.toStdString());
    }
    catch (const std::exception& e)
    {
        emit emulationFatalError(e.what());
        return;
    }

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
    constexpr auto frameDuration{24740000ns};

    _running = true;

    const auto now{std::chrono::steady_clock::now()};

    try
    {
        while (_running)
        {
            _components.cpu.runInstruction();
        }
    }
    catch (const std::exception& e)
    {
        emit emulationFatalError(e.what());
    }

    if (const auto diff{now - _lastUpdate}; diff < frameDuration)
    {
        std::this_thread::sleep_for(frameDuration - diff);
    }

    _lastUpdate = now;
}

void Emulator::onRender(const Graphics::Framebuffer& framebuffer)
{
    _running = false;

    /* This signal will be delivered via a QueuedConnection and won't block here. */

    emit frameReady(framebuffer);
}

Emulator::Components::Components(IRenderer& renderer)
    : _state(), bus(_state), timer(bus), ppu(bus, renderer), cpu(_state, bus, timer, ppu), echoRam(workRam)
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