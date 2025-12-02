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
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

  public slots:
    void onFrameReady(const Graphics::Framebuffer& framebuffer);
    void onEmulationFatalError(const QString& message);

  signals:
    void keyPressed(Key key);
    void keyReleased(Key key);

    void requestNextFrame();
    void requestLoadRom(const QString& path);

  private:
    static constexpr qsizetype MaxRecentFiles{8};

    void               _updateDisplay(const Graphics::Framebuffer& framebuffer) const;
    std::optional<Key> _isAMappedKey(const QKeyEvent* keyEvent) const;

    void _populateRecentMenu();
    void _loadSettings();
    void _addRecentFile(const QString& path);
    void _clearRecentFiles();

    QMap<QKeySequence, Key> _keyMapping;
    QThread                 _emulatorThread;
    Ui::MainWindow*         _ui;
};

#endif  // GBEMU_MAINWINDOW_HXX