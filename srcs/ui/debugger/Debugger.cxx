//
// Created by plouvel on 12/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Debugger.h" resolved

#include "../../../includes/ui/debugger/Debugger.hxx"

#include <QFontDatabase>

#include "../../../includes/ui/debugger/RegisterModel.hxx"
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

    ui->instructionsDisassembly->setModel(&_instructionsModel);
    ui->instructionsDisassembly->setItemDelegate(new BreakpointDelegate{this});
    ui->instructionsDisassembly->setUniformItemSizes(true);

    ui->instructionsDisassembly->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

Debugger::~Debugger()
{
    delete ui;
}

void Debugger::onCpuRegisterChanged(RegisterModel::RegisterEntry registerEntry) {}

void Debugger::onCpuFlagsChanged(bool checked) {}

void Debugger::onCpuImeChanged(bool checked) {}

void Debugger::onPpuRegisterChanged(RegisterModel::RegisterEntry registerEntry) {}

void Debugger::onUpdate(const Emulator::State& state)
{
    _cpuEightBitsRegistersModel.setRegisterValue("A", state.cpuView.registers.A);
    _cpuEightBitsRegistersModel.setRegisterValue("B", state.cpuView.registers.B);
    _cpuEightBitsRegistersModel.setRegisterValue("C", state.cpuView.registers.C);
    _cpuEightBitsRegistersModel.setRegisterValue("D", state.cpuView.registers.D);
    _cpuEightBitsRegistersModel.setRegisterValue("H", state.cpuView.registers.H);
    _cpuEightBitsRegistersModel.setRegisterValue("L", state.cpuView.registers.L);
    _cpuEightBitsRegistersModel.setRegisterValue("IE", state.cpuView.interrupts.IE);
    _cpuEightBitsRegistersModel.setRegisterValue("IF", state.cpuView.interrupts.IF);

    _cpuSixteenBitsRegistersModel.setRegisterValue("AF", state.cpuView.registers.AF);
    _cpuSixteenBitsRegistersModel.setRegisterValue("BC", state.cpuView.registers.BC);
    _cpuSixteenBitsRegistersModel.setRegisterValue("DE", state.cpuView.registers.DE);
    _cpuSixteenBitsRegistersModel.setRegisterValue("HL", state.cpuView.registers.HL);
    _cpuSixteenBitsRegistersModel.setRegisterValue("SP", state.cpuView.registers.SP);
    _cpuSixteenBitsRegistersModel.setRegisterValue("PC", state.cpuView.registers.PC);

    ui->zeroFlag->setChecked(state.cpuView.flags.zero);
    ui->carryFlag->setChecked(state.cpuView.flags.carry);
    ui->halfCarryFlag->setChecked(state.cpuView.flags.halfCarry);
    ui->negativeFlag->setChecked(state.cpuView.flags.subtract);
    ui->ime->setChecked(state.cpuView.interrupts.IME);

    _ppuRegistersModel.setRegisterValue("LCDC", state.busView[MemoryMap::IORegisters::LCDC]);
    _ppuRegistersModel.setRegisterValue("STAT", state.busView[MemoryMap::IORegisters::STAT]);
    _ppuRegistersModel.setRegisterValue("SCY", state.busView[MemoryMap::IORegisters::SCY]);
    _ppuRegistersModel.setRegisterValue("SCX", state.busView[MemoryMap::IORegisters::SCX]);
    _ppuRegistersModel.setRegisterValue("LY", state.busView[MemoryMap::IORegisters::LY]);
    _ppuRegistersModel.setRegisterValue("LYC", state.busView[MemoryMap::IORegisters::LYC]);
    _ppuRegistersModel.setRegisterValue("BGP", state.busView[MemoryMap::IORegisters::BGP]);
    _ppuRegistersModel.setRegisterValue("OBP0", state.busView[MemoryMap::IORegisters::OBP0]);
    _ppuRegistersModel.setRegisterValue("OBP1", state.busView[MemoryMap::IORegisters::OBP1]);
    _ppuRegistersModel.setRegisterValue("WX", state.busView[MemoryMap::IORegisters::WX]);
    _ppuRegistersModel.setRegisterValue("WY", state.busView[MemoryMap::IORegisters::WY]);

    _instructionsModel.updateInstructions(state.busView);

    const auto idx = _instructionsModel.indexForAddress(state.cpuView.registers.PC);
    auto*      sm  = ui->instructionsDisassembly->selectionModel();
    sm->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->instructionsDisassembly->scrollTo(idx, QAbstractItemView::PositionAtCenter);
}
