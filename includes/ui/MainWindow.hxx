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

    void showEvent(QShowEvent* event) override;

  public slots:
    void onFrameReady(const Graphics::Framebuffer& framebuffer);

  signals:
    void requestNextFrame();
    void requestLoadRom(const QString& path);

  private:
    static constexpr qsizetype MaxRecentFiles{8};

    void updateDisplay(const Graphics::Framebuffer& framebuffer) const;

    void populateRecentMenu();
    void addRecentFile(const QString& path);
    void clearRecentFiles();

    QThread         _emulatorThread;
    Ui::MainWindow* _ui;
};

#endif  // GBEMU_MAINWINDOW_HXX
