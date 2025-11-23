//
// Created by plouvel on 23/11/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ui/MainWindow.hxx"

#include <iostream>

#include "Emulator.hxx"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _emulator(new Emulator), _ui(new Ui::MainWindow)
{
    _emulator->moveToThread(&_emulatorThread);

    connect(&_emulatorThread, &QThread::started, _emulator, &Emulator::run);
    connect(&_emulatorThread, &QThread::finished, _emulator, &QObject::deleteLater);
    connect(this, &MainWindow::closeEvent, _emulator, &Emulator::end);
    connect(_emulator, &Emulator::frameReady, this, &MainWindow::onFrameReady);

    _emulatorThread.start();

    _ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    _emulatorThread.quit();
    _emulatorThread.wait();

    delete _ui;
}

void MainWindow::onFrameReady()
{
    std::cout << "Frame ready!" << std::endl;
}
