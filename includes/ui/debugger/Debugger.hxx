//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <QMainWindow>

#include "RegisterModel.hxx"
#include "BreakpointDelegate.hxx"
#include "InstructionModel.hxx"

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

  private slots:
    void onCpuRegisterChanged(RegisterModel::RegisterEntry registerEntry);
    void onCpuFlagsChanged(bool checked);
    void onCpuImeChanged(bool checked);
    void onPpuRegisterChanged(RegisterModel::RegisterEntry registerEntry);

  signals:

    void pauseExecution();
    void continueExecution();
    void stepIn();
    void stepOver();
    void stepOut();

  private:
    Ui::Debugger*    ui;
    RegisterModel    _cpuEightBitsRegistersModel{};
    RegisterModel    _cpuSixteenBitsRegistersModel{};
    RegisterModel    _ppuRegistersModel{};
    InstructionModel _instructionsModel{};
};

#endif  // GBEMU_DEBUGGER_HXX
