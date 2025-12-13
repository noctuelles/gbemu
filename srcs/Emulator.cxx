//
// Created by plouvel on 23/11/2025.
//

#include "Emulator.hxx"

#include <QFile>
#include <QThread>
#include <QTimer>
#include <iostream>

Emulator::Emulator(const std::optional<QString>& bootRomPath, QObject* parent)
    : QObject(parent), _renderer(new QtRenderer(this)), _components(*_renderer), _backEndDebugger(_components.cpu)
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

void Emulator::setBreakpoint(const Debugger::Breakpoint& breakpoint)
{
    _backEndDebugger.addBreakpoint(breakpoint);
}

void Emulator::getEmulationStatus()
{
    emit emulationStatusUpdated({_components.cpu.getView(), _components.bus.getView()});
}

void Emulator::runFrame()
{
    const auto frameStart{std::chrono::steady_clock::now()};

    try
    {
        _running = true;

        while (_running)
        {
            if (nextInstruction())
            {
                return;
            }
        }
    }
    catch (const std::exception& e)
    {
        emit emulationFatalError(e.what());
        return;
    }

    const auto frameEnd{std::chrono::steady_clock::now()};

    if (const auto emulationTime{frameEnd - frameStart}; emulationTime < _frameDuration)
    {
        /* Not using QT sleep function here. The event loop is blocked, but this is not an issue. */
        const auto sleepTime{_frameDuration - emulationTime};
        std::this_thread::sleep_for(sleepTime);
    }
}

bool Emulator::nextInstruction()
{
    _components.cpu.runInstruction();

    if (_backEndDebugger.shouldBreak())
    {
        emit breakpointHit({_components.cpu.getView(), _components.bus.getView()});
        return true;
    }

    return false;
}

void Emulator::stepInInstruction()
{
    _components.cpu.runInstruction();

    emit emulationStatusUpdated({_components.cpu.getView(), _components.bus.getView()});
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