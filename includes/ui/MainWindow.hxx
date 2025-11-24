//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_MAINWINDOW_HXX
#define GBEMU_MAINWINDOW_HXX

#include <QMainWindow>
#include <QThread>

#include "Emulator.hxx"
#include "QtRenderer.hxx"

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

    void showEvent(QShowEvent* event) override;

  public slots:
    void onFrameReady(const Graphics::Framebuffer& framebuffer);

  signals:
    void nextFrame();
    void loadRomRequested(const QString& path);

  private:
    void updateDisplay(const Graphics::Framebuffer& framebuffer) const;

    QThread         _emulatorThread;
    Ui::MainWindow* _ui;
};

#endif  // GBEMU_MAINWINDOW_HXX
