//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMetaEnum>
#include <QSettings>
#include <iostream>

#include "Emulator.hxx"
#include "ui/Preference.hxx"
#include "ui/Settings.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow), _debugger(this)
{
    _ui->setupUi(this);

    _ui->display->setMinimumSize(std::tuple_size_v<Graphics::Framebuffer::value_type> * 2,
                                 std::tuple_size_v<Graphics::Framebuffer> * 2);
    resize(_ui->display->minimumSize());

    _emulationStatusLabel = new QLabel{statusBar()};
    statusBar()->addPermanentWidget(_emulationStatusLabel);
    _updateEmulationStatus(Status::Stopped);

    _populateRecentMenu();
    _loadSettings();

    installEventFilter(this);

    /* UI */

    connect(_ui->actionOpen, &QAction::triggered, this,
            [this]
            {
                if (const auto path = QFileDialog::getOpenFileName(this, tr("Open ROM"), ".", tr("ROM Files (*.gb)"));
                    !path.isEmpty())
                {
                    _addRecentFile(path);
                    _startEmulation(path);
                }
            });

    connect(_ui->actionPreference, &QAction::triggered, this,
            [this]
            {
                if (Preference prefDialog(this); prefDialog.exec() == QDialog::Accepted)
                {
                    _loadSettings();
                }
            });

    connect(_ui->actionDebugger, &QAction::triggered, this, [this] { _debugger.show(); });

    connect(&_debugger, &Debugger::pauseExecution, this,
            [this]
            {
                if (_emulationStatus == Status::Running)
                {
                    _updateEmulationStatus(Status::Paused);
                }
            });

    connect(&_debugger, &Debugger::continueExecution, this,
            [this]
            {
                if (_emulationStatus == Status::Paused)
                {
                    _updateEmulationStatus(Status::Running);
                    emit requestNextFrame();
                }
            });

    connect(&_debugger, &Debugger::stepIn, this, [this] { emit requestSetBreakpoint(0x100); });
}

MainWindow::~MainWindow()
{
    if (_emulatorThread.isRunning())
    {
        _emulatorThread.quit();
        _emulatorThread.wait();
    }

    delete _ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    constexpr Graphics::Framebuffer framebuffer{};

    QMainWindow::showEvent(event);

    _updateDisplay(framebuffer);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (const auto key{_isAMappedKey(event)}; key.has_value())
    {
        emit keyPressed(key.value());
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (const auto key{_isAMappedKey(event)}; key.has_value())
    {
        emit keyReleased(key.value());
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (_emulationStatus == Status::Paused)
    {
        assert(_framebuffer != nullptr);
        _updateDisplay(*_framebuffer);
    }
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Show)
    {
        qDebug() << "Show event";
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onBreakpointHit()
{
    _updateEmulationStatus(Status::Paused);
}

void MainWindow::onFrameReady(const Graphics::Framebuffer& framebuffer)
{
    _updateDisplay(framebuffer);

    _framebuffer = &framebuffer;

    if (_emulationStatus == Status::Paused)
    {
        return;
    }

    emit requestNextFrame();
}

void MainWindow::onEmulationFatalError(const QString& message)
{
    constexpr Graphics::Framebuffer framebuffer{};

    QMessageBox::critical(nullptr, tr("Emulation fatal error"), QString{"%1\nHalting."}.arg(message));

    _updateDisplay(framebuffer);
    _updateEmulationStatus(Status::Stopped);
}

void MainWindow::_updateDisplay(const Graphics::Framebuffer& framebuffer) const
{
    QImage img{std::tuple_size_v<Graphics::Framebuffer::value_type>, std::tuple_size_v<Graphics::Framebuffer>,
               QImage::Format_RGB32};

    for (const auto& [y, row] : std::views::enumerate(framebuffer))
    {
        for (const auto& [x, color] : std::views::enumerate(row))
        {
            /* Only extract the pixel and leave out the debugging information. */

            const auto& rgbColor{_colorMapping[color & 0b11]};
            img.setPixel(x, y, qRgb(rgbColor.red(), rgbColor.green(), rgbColor.blue()));
        }
    }

    _ui->display->setPixmap(
        QPixmap::fromImage(img.scaled(_ui->display->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation)));
}

std::optional<Key> MainWindow::_isAMappedKey(const QKeyEvent* keyEvent) const
{
    if (!keyEvent->isAutoRepeat())
    {
        const auto keySequence{QKeySequence(static_cast<int>(keyEvent->modifiers()) | keyEvent->key())};

        if (const auto it{_keyMapping.find(keySequence)}; it != _keyMapping.end())
        {
            return it.value();
        }

        return std::nullopt;
    }

    return std::nullopt;
}

void MainWindow::_startEmulation(const QString& romPath)
{
    if (_emulatorThread.isRunning())
    {
        _emulatorThread.quit();
        _emulatorThread.wait();
    }

    const auto bootRomPath{Settings::isBootRomEnabled() ? std::optional{Settings::getBootRomPath()} : std::nullopt};
    const auto emulator{new Emulator{bootRomPath}};

    emulator->moveToThread(&_emulatorThread);

    connect(&_emulatorThread, &QThread::finished, emulator, &QObject::deleteLater);

    connect(emulator, &Emulator::frameReady, this, &MainWindow::onFrameReady);
    connect(emulator, &Emulator::emulationFatalError, this, &MainWindow::onEmulationFatalError);
    connect(emulator, &Emulator::breakpointHit, this, &MainWindow::onBreakpointHit);

    connect(this, &MainWindow::requestNextFrame, emulator, &Emulator::runFrame);

    connect(this, &MainWindow::keyPressed, emulator, &Emulator::onKeyPressed);
    connect(this, &MainWindow::keyReleased, emulator, &Emulator::onKeyReleased);

    connect(this, &MainWindow::requestStartEmulation, emulator, &Emulator::startEmulation);
    connect(this, &MainWindow::requestSetBreakpoint, emulator, &Emulator::setBreakpoint);

    _emulatorThread.start();
    _updateEmulationStatus(Status::Running);

    emit requestStartEmulation(romPath);
}

void MainWindow::_updateEmulationStatus(const Status status)
{
    const auto metaEnum{QMetaEnum::fromType<Status>()};
    const auto text{metaEnum.valueToKey(std::to_underlying(status))};

    _emulationStatus = status;
    _emulationStatusLabel->setText(text);
}

void MainWindow::_populateRecentMenu()
{
    QSettings settings;

    QStringList cleanedStoredROM{};
    const auto  storedROM{settings.value("recentFiles").toStringList()};
    const auto  clear{new QAction(tr("Clear"), _ui->menuOpen_Recent)};

    if (_ui->menuOpen_Recent)
    {
        _ui->menuOpen_Recent->clear();

        for (const auto& rom : storedROM)
        {
            const QFileInfo fi(rom);

            if (!fi.exists())
            {
                continue;
            }

            const auto action{new QAction(fi.fileName(), _ui->menuOpen_Recent)};

            action->setData(fi.absoluteFilePath());
            connect(action, &QAction::triggered, this,
                    [this, path = fi.absoluteFilePath()]
                    {
                        if (!QFileInfo::exists(path))
                        {
                            QMessageBox::warning(this, tr("File not found"),
                                                 tr("The file \"%1\" cannot be found.").arg(path));
                            _populateRecentMenu();
                            return;
                        }

                        _addRecentFile(path);
                        _startEmulation(path);
                    });

            _ui->menuOpen_Recent->addAction(action);

            cleanedStoredROM.emplace_back(fi.absoluteFilePath());
        }

        if (_ui->menuOpen_Recent->isEmpty())
        {
            const auto placeholder = new QAction(tr("(Empty)"), _ui->menuOpen_Recent);
            placeholder->setEnabled(false);
            _ui->menuOpen_Recent->addAction(placeholder);
        }

        _ui->menuOpen_Recent->addSeparator();
        connect(clear, &QAction::triggered, this, &MainWindow::_clearRecentFiles);
        _ui->menuOpen_Recent->addAction(clear);

        settings.setValue("recentFiles", cleanedStoredROM);
    }
}

void MainWindow::_loadSettings()
{
    {
        using namespace Settings::Keys;

        _keyMapping.clear();

        _keyMapping.insert(get(Key::Up), Key::Up);
        _keyMapping.insert(get(Key::Down), Key::Down);
        _keyMapping.insert(get(Key::Left), Key::Left);
        _keyMapping.insert(get(Key::Right), Key::Right);

        _keyMapping.insert(get(Key::A), Key::A);
        _keyMapping.insert(get(Key::B), Key::B);
        _keyMapping.insert(get(Key::Select), Key::Select);
        _keyMapping.insert(get(Key::Start), Key::Start);
    }

    {
        using namespace Settings::Palette;

        _colorMapping[0] = get(Type::Color0);
        _colorMapping[1] = get(Type::Color1);
        _colorMapping[2] = get(Type::Color2);
        _colorMapping[3] = get(Type::Color3);
    }
}

void MainWindow::_addRecentFile(const QString& path)
{
    const QString absPath{QFileInfo(path).absoluteFilePath()};
    QSettings     settings;
    QStringList   list{settings.value("recentFiles").toStringList()};

    list.removeAll(absPath);
    list.prepend(absPath);

    while (list.size() > MaxRecentFiles)
    {
        list.removeLast();
    }

    settings.setValue("recentFiles", list);

    _populateRecentMenu();
}

void MainWindow::_clearRecentFiles()
{
    QSettings settings;

    settings.remove("recentFiles");

    _populateRecentMenu();
}
