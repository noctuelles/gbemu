//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_DEBUGGER_HXX
#define GBEMU_DEBUGGER_HXX

#include <QMainWindow>

#include "RegisterModel.hxx"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Debugger;
}
QT_END_NAMESPACE

class Debugger : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Debugger(QWidget* parent = nullptr);
    ~Debugger() override;

  signals:

    void pauseExecution();
    void continueExecution();
    void stepIn();
    void stepOver();
    void stepOut();

  private:
    Ui::Debugger* ui;
    RegisterModel _cpuEightBitsRegistersModel{};
    RegisterModel _cpuSixteenBitsRegistersModel{};
    RegisterModel _ppuRegistersModel{};
};

#endif  // GBEMU_DEBUGGER_HXX
