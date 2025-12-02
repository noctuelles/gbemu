//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <QFileDialog>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <iostream>

#include "ui/Preference.hxx"
#include "Emulator.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    const auto emulator{new Emulator};

    _ui->setupUi(this);

    _ui->display->setMinimumSize(std::tuple_size_v<Graphics::Framebuffer::value_type> * 2,
                                 std::tuple_size_v<Graphics::Framebuffer> * 2);
    resize(_ui->display->minimumSize());

    _populateRecentMenu();
    _loadSettings();

    emulator->moveToThread(&_emulatorThread);

    connect(&_emulatorThread, &QThread::finished, emulator, &QObject::deleteLater);

    connect(emulator, &Emulator::frameReady, this, &MainWindow::onFrameReady);
    connect(this, &MainWindow::requestNextFrame, emulator, &Emulator::runFrame);

    connect(this, &MainWindow::keyPressed, emulator, &Emulator::onKeyPressed);
    connect(this, &MainWindow::keyReleased, emulator, &Emulator::onKeyReleased);

    connect(this, &MainWindow::requestLoadRom, emulator, &Emulator::loadRom);

    /* UI */

    connect(_ui->actionOpen, &QAction::triggered, this,
            [this]
            {
                if (const auto path = QFileDialog::getOpenFileName(this, tr("Open ROM"), ".", tr("ROM Files (*.gb)"));
                    !path.isEmpty())
                {
                    _addRecentFile(path);
                    emit requestLoadRom(path);
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

    _emulatorThread.start();
}

MainWindow::~MainWindow()
{
    _emulatorThread.quit();
    _emulatorThread.wait();

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

void MainWindow::onFrameReady(const Graphics::Framebuffer& framebuffer)
{
    _updateDisplay(framebuffer);
    emit requestNextFrame();
}

void MainWindow::_updateDisplay(const Graphics::Framebuffer& framebuffer) const
{
    QImage img{std::tuple_size_v<Graphics::Framebuffer::value_type>, std::tuple_size_v<Graphics::Framebuffer>,
               QImage::Format_RGB32};

    for (const auto& [y, row] : std::views::enumerate(framebuffer))
    {
        for (const auto& [x, color] : std::views::enumerate(row))
        {
            const auto colorValue{color};
            QRgb       rgbColor{};

            if (colorValue == 0b11)
            {
                rgbColor = qRgb(0x08, 0x18, 0x20);
            }
            else if (colorValue == 0b10)
            {
                rgbColor = qRgb(0x34, 0x68, 0x56);
            }
            else if (colorValue == 0b01)
            {
                rgbColor = qRgb(0x88, 0xC0, 0x70);
            }
            else if (colorValue == 0b00)
            {
                rgbColor = qRgb(0xE0, 0xF8, 0xD8);
            }

            img.setPixel(x, y, rgbColor);
        }
    }

    _ui->display->setPixmap(
        QPixmap::fromImage(img.scaled(_ui->display->size(), Qt::IgnoreAspectRatio, Qt::FastTransformation)));
}

std::optional<Emulator::Key> MainWindow::_isAMappedKey(const QKeyEvent* keyEvent) const
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
                        emit requestLoadRom(path);
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
    QSettings settings{};

    settings.beginGroup("preference/keys");

    _keyMapping.clear();

    _keyMapping.insert(settings.value("up").value<QKeySequence>()[0], Emulator::Key::Up);
    _keyMapping.insert(settings.value("down").value<QKeySequence>()[0], Emulator::Key::Down);
    _keyMapping.insert(settings.value("left").value<QKeySequence>()[0], Emulator::Key::Left);
    _keyMapping.insert(settings.value("right").value<QKeySequence>()[0], Emulator::Key::Right);
    _keyMapping.insert(settings.value("a").value<QKeySequence>()[0], Emulator::Key::A);
    _keyMapping.insert(settings.value("b").value<QKeySequence>()[0], Emulator::Key::B);
    _keyMapping.insert(settings.value("select").value<QKeySequence>()[0], Emulator::Key::Select);
    _keyMapping.insert(settings.value("start").value<QKeySequence>()[0], Emulator::Key::Start);

    settings.endGroup();
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
