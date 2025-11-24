//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <QFileDialog>

#include "Emulator.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    const auto emulator{new Emulator};

    _ui->setupUi(this);

    emulator->moveToThread(&_emulatorThread);

    connect(&_emulatorThread, &QThread::finished, emulator, &QObject::deleteLater);

    connect(emulator, &Emulator::frameReady, this, &MainWindow::onFrameReady);
    connect(this, &MainWindow::requestNextFrame, emulator, &Emulator::runFrame);

    connect(this, &MainWindow::requestLoadRom, emulator, &Emulator::loadRom);
    connect(_ui->actionLoad_ROM, &QAction::triggered, this,
            [this]
            {
                if (const auto path = QFileDialog::getOpenFileName(this, tr("Open ROM"), ".", tr("ROM Files (*.gb)"));
                    !path.isEmpty())
                {
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