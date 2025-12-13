//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <QMainWindow>

#include "BreakpointDelegate.hxx"
#include "Emulator.hxx"
#include "InstructionModel.hxx"
#include "RegisterModel.hxx"
#include "StackFrameModel.hxx"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Debugger;
}
QT_END_NAMESPACE

class Debugger final : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Debugger(QWidget* parent = nullptr);
    ~Debugger() override;

    void setEnabled(bool enabled) const;

  public slots:
    void onEmulationStateUpdate(const Emulator::State& state);

  private slots:
    void onCpuRegisterChanged(const QString& name, uint64_t value);
    void onPpuRegisterChanged(const QString& name, uint64_t value);
    void onCpuFlagsChanged(bool checked);
    void onCpuImeChanged(bool checked);

  signals:

    void pauseExecution();
    void continueExecution();
    void stepIn();
    void stepOver();
    void stepOut();
    void updateEmulationState(const Emulator::State& state);

  private:
    void _scrollToAddress(uint16_t address) const;
    void _selectAddress(uint16_t address) const;

    Ui::Debugger*    ui;

    Emulator::State  _currentState{};
    RegisterModel    _cpuEightBitsRegistersModel{};
    RegisterModel    _cpuSixteenBitsRegistersModel{};
    RegisterModel    _ppuRegistersModel{};
    StackFrameModel  _stackModel{};
    InstructionModel _instructionsModel{};
};

#endif  // GBEMU_DEBUGGER_HXX
