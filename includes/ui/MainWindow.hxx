//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_MAINWINDOW_HXX
#define GBEMU_MAINWINDOW_HXX

#include <QMainWindow>
#include <QThread>

#include "Emulator.hxx"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

  public slots:

    void onFrameReady();

  private:
    Emulator*       _emulator;
    QThread         _emulatorThread;
    Ui::MainWindow* _ui;
};

#endif  // GBEMU_MAINWINDOW_HXX
