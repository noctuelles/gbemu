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

#include "Emulator.hxx"
#include "Preference.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    const auto emulator{new Emulator};

    _ui->setupUi(this);

    _ui->display->setMinimumSize(std::tuple_size_v<Graphics::Framebuffer::value_type> * 2,
                                 std::tuple_size_v<Graphics::Framebuffer> * 2);
    resize(_ui->display->minimumSize());

    populateRecentMenu();

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
                    addRecentFile(path);
                    emit requestLoadRom(path);
                }
            });

    connect(_ui->actionPreference, &QAction::triggered, this,
            [this]
            {
                Preference prefDialog(this);

                prefDialog.exec();
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
    updateDisplay(framebuffer);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
            case Qt::Key_Up:
                emit keyPressed(Emulator::Key::Up);
                break;
            case Qt::Key_Down:
                emit keyPressed(Emulator::Key::Down);
                break;
            case Qt::Key_Left:
                emit keyPressed(Emulator::Key::Left);
                break;
            case Qt::Key_Right:
                emit keyPressed(Emulator::Key::Right);
                break;
            default:
                break;
        }
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        switch (event->key())
        {
            case Qt::Key_Up:
                emit keyReleased(Emulator::Key::Up);
                break;
            case Qt::Key_Down:
                emit keyReleased(Emulator::Key::Down);
                break;
            case Qt::Key_Left:
                emit keyReleased(Emulator::Key::Left);
                break;
            case Qt::Key_Right:
                emit keyReleased(Emulator::Key::Right);
                break;
            default:
                break;
        }
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::onFrameReady(const Graphics::Framebuffer& framebuffer)
{
    updateDisplay(framebuffer);
    emit requestNextFrame();
}

void MainWindow::updateDisplay(const Graphics::Framebuffer& framebuffer) const
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

void MainWindow::populateRecentMenu()
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
                            populateRecentMenu();
                            return;
                        }

                        addRecentFile(path);
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
        connect(clear, &QAction::triggered, this, &MainWindow::clearRecentFiles);
        _ui->menuOpen_Recent->addAction(clear);

        settings.setValue("recentFiles", cleanedStoredROM);
    }
}

void MainWindow::addRecentFile(const QString& path)
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

    populateRecentMenu();
}

void MainWindow::clearRecentFiles()
{
    QSettings settings;

    settings.remove("recentFiles");

    populateRecentMenu();
}
