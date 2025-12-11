//
// Created by plouvel on 12/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Debugger.h" resolved

#include "ui/Debugger.hxx"

#include "ui/RegisterModel.hxx"
#include "ui_Debugger.h"

Debugger::Debugger(QWidget* parent) : QMainWindow(parent), ui(new Ui::Debugger), _cpuEightBitsRegistersModel()
{
    ui->setupUi(this);

    connect(ui->actionPause, &QAction::triggered, this, &Debugger::pauseExecution);
    connect(ui->actionContinue, &QAction::triggered, this, &Debugger::continueExecution);
    connect(ui->actionStep_In, &QAction::triggered, this, &Debugger::stepIn);

    std::vector<RegisterModel::RegisterEntry> registersEight{
        {"A", 0xFF, RegisterModel::RegisterEntry::Size::Eight}, {"B", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"C", 0xAB, RegisterModel::RegisterEntry::Size::Eight}, {"D", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"H", 0, RegisterModel::RegisterEntry::Size::Eight},    {"L", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"IE", 0, RegisterModel::RegisterEntry::Size::Eight},   {"IF", 0, RegisterModel::RegisterEntry::Size::Eight},
    };

    std::vector<RegisterModel::RegisterEntry> registersSixteen{
        {"AF", 0xFFAA, RegisterModel::RegisterEntry::Size::Sixteen},
        {"BC", 0, RegisterModel::RegisterEntry::Size::Sixteen},
        {"DE", 0xABEE, RegisterModel::RegisterEntry::Size::Sixteen},
        {"HL", 0, RegisterModel::RegisterEntry::Size::Sixteen},
        {"SP", 0, RegisterModel::RegisterEntry::Size::Sixteen},
        {"PC", 0, RegisterModel::RegisterEntry::Size::Sixteen},
    };

    std::vector<RegisterModel::RegisterEntry> ppuRegisters{
        {"LCDC", 0x00, RegisterModel::RegisterEntry::Size::Eight},
        {"STAT", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"SCY", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"SCX", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"LY", 0, RegisterModel::RegisterEntry::Size::Eight, false},
        {"LYC", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"BGP", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"OBP0", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"OBP1", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"WX", 0, RegisterModel::RegisterEntry::Size::Eight},
        {"WY", 0, RegisterModel::RegisterEntry::Size::Eight},
    };

    _cpuEightBitsRegistersModel.setRegisters(registersEight);
    ui->eightBitsCpuRegisters->setModel(&_cpuEightBitsRegistersModel);

    _cpuSixteenBitsRegistersModel.setRegisters(registersSixteen);
    ui->sixteenBitsCpuRegisters->setModel(&_cpuSixteenBitsRegistersModel);

    _ppuRegistersModel.setRegisters(ppuRegisters);
    ui->ppuRegisters->setModel(&_ppuRegistersModel);
}

Debugger::~Debugger()
{
    delete ui;
}
