//
// Created by plouvel on 23/11/2025.
//

#ifndef GBEMU_MAINWINDOW_HXX
#define GBEMU_MAINWINDOW_HXX

#include <QLabel>
#include <QMainWindow>
#include <QThread>

#include "Debugger.hxx"
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
    bool eventFilter(QObject* watched, QEvent* event) override;

  public slots:
    void onFrameReady(const Graphics::Framebuffer& framebuffer);
    void onEmulationFatalError(const QString& message) const;

  signals:
    void keyPressed(Key key);
    void keyReleased(Key key);

    void requestNextFrame();
    void requestStartEmulation(const QString& path);

  private:
    enum class Status
    {
        Running,
        Stopped,
        Paused,
    };
    Q_ENUM(Status)

    static constexpr qsizetype MaxRecentFiles{8};

    void               _updateDisplay(const Graphics::Framebuffer& framebuffer) const;
    std::optional<Key> _isAMappedKey(const QKeyEvent* keyEvent) const;

    void _startEmulation(const QString& romPath);
    void _updateEmulationStatus(Status status) const;
    void _populateRecentMenu();
    void _loadSettings();
    void _addRecentFile(const QString& path);
    void _clearRecentFiles();

    QMap<QKeySequence, Key> _keyMapping;
    std::array<QColor, 4>   _colorMapping;
    QLabel*                 _emulationStatus;
    QThread                 _emulatorThread;
    Ui::MainWindow*         _ui;

    Debugger _debugger;
};

#endif  // GBEMU_MAINWINDOW_HXX