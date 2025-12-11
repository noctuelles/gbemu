//
// Created by plouvel on 12/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Debugger.h" resolved

#include "ui/Debugger.hxx"

#include "ui/RegisterModel.hxx"
#include "ui_Debugger.h"

Debugger::Debugger(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::Debugger),
      _cpuEightBitsRegistersModel{{
          {"A", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"B", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"C", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"D", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"H", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"L", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"IE", 0, RegisterModel::RegisterEntry::Size::Eight},
          {"IF", 0, RegisterModel::RegisterEntry::Size::Eight},
      }},
      _cpuSixteenBitsRegistersModel({
          {"AF", 0, RegisterModel::RegisterEntry::Size::Sixteen},
          {"BC", 0, RegisterModel::RegisterEntry::Size::Sixteen},
          {"DE", 0, RegisterModel::RegisterEntry::Size::Sixteen},
          {"HL", 0, RegisterModel::RegisterEntry::Size::Sixteen},
          {"SP", 0, RegisterModel::RegisterEntry::Size::Sixteen},
          {"PC", 0, RegisterModel::RegisterEntry::Size::Sixteen},
      }),
      _ppuRegistersModel({

          {"LCDC", 0, RegisterModel::RegisterEntry::Size::Eight},
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
      })
{
    ui->setupUi(this);

    connect(ui->actionPause, &QAction::triggered, this, &Debugger::pauseExecution);
    connect(ui->actionContinue, &QAction::triggered, this, &Debugger::continueExecution);
    connect(ui->actionStep_In, &QAction::triggered, this, &Debugger::stepIn);

    ui->eightBitsCpuRegisters->setModel(&_cpuEightBitsRegistersModel);
    ui->sixteenBitsCpuRegisters->setModel(&_cpuSixteenBitsRegistersModel);
    ui->ppuRegisters->setModel(&_ppuRegistersModel);

    connect(&_cpuEightBitsRegistersModel, &RegisterModel::registerChanged, this, &Debugger::onCpuRegisterChanged);
    connect(&_cpuSixteenBitsRegistersModel, &RegisterModel::registerChanged, this, &Debugger::onCpuRegisterChanged);
    connect(&_ppuRegistersModel, &RegisterModel::registerChanged, this, &Debugger::onPpuRegisterChanged);

    connect(ui->carryFlag, &QCheckBox::toggled, this, &Debugger::onCpuFlagsChanged);
    connect(ui->zeroFlag, &QCheckBox::toggled, this, &Debugger::onCpuFlagsChanged);
    connect(ui->halfCarryFlag, &QCheckBox::toggled, this, &Debugger::onCpuFlagsChanged);
    connect(ui->negativeFlag, &QCheckBox::toggled, this, &Debugger::onCpuFlagsChanged);
    connect(ui->ime, &QCheckBox::toggled, this, &Debugger::onCpuImeChanged);
}

Debugger::~Debugger()
{
    delete ui;
}

void Debugger::onCpuRegisterChanged(RegisterModel::RegisterEntry registerEntry) {}

void Debugger::onCpuFlagsChanged(bool checked) {}

void Debugger::onCpuImeChanged(bool checked) {}

void Debugger::onPpuRegisterChanged(RegisterModel::RegisterEntry registerEntry) {}
