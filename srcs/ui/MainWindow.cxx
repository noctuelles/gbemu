//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>

#include "Emulator.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    const auto emulator{new Emulator};

    _ui->setupUi(this);

    populateRecentMenu();

    emulator->moveToThread(&_emulatorThread);

    connect(&_emulatorThread, &QThread::finished, emulator, &QObject::deleteLater);

    connect(emulator, &Emulator::frameReady, this, &MainWindow::onFrameReady);
    connect(this, &MainWindow::requestNextFrame, emulator, &Emulator::runFrame);

    connect(this, &MainWindow::requestLoadRom, emulator, &Emulator::loadRom);
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
            const auto colorValue{color.to_ulong()};
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

            const auto recentROM{new QAction(fi.fileName(), _ui->menuOpen_Recent)};

            recentROM->setData(fi.absoluteFilePath());
            connect(recentROM, &QAction::triggered, this,
                    [this, path = fi.absoluteFilePath()]
                    {
                        if (!QFileInfo::exists(path))
                        {
                            QMessageBox::warning(this, tr("File not found"),
                                                 tr("The file \"%1\" cannot be found.").arg(path));
                            populateRecentMenu();
                            return;
                        }

                        emit requestLoadRom(path);
                        addRecentFile(path);
                    });

            _ui->menuOpen_Recent->addAction(recentROM);

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
