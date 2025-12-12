//
// Created by plouvel on 12/11/25.
//

#include "ui/debugger/InstructionModel.hxx"

#include <QColor>

#include "Common.hxx"
#include "hardware/core/SM83.hxx"

InstructionModel::InstructionModel(QObject* parent) : QAbstractListModel(parent) {}

void InstructionModel::updateInstructions(const AddressSpace& addressSpace)
{
    beginResetModel();

    const SM83::Disassembler disassembler{addressSpace};

    _instructions = disassembler.disassemble();

    endResetModel();
}

QModelIndex InstructionModel::indexForAddress(uint16_t address) const
{
    const auto it{std::ranges::find_if(_instructions,
                                       [address](const SM83::Disassembler::DisassembledInstruction& instruction)
                                       { return instruction.address == address; })};
    if (it == _instructions.end())
    {
        return QModelIndex{};
    }

    return index(std::distance(std::begin(_instructions), it), 0);
}

int InstructionModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int InstructionModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(_instructions.size());
}

QVariant InstructionModel::data(const QModelIndex& index, int role) const
{
    if (checkIndex(index) == false)
    {
        return {};
    }
    const auto& [address, opcode, name]{_instructions.at(index.row())};

    if (role == Qt::DisplayRole)
    {
        return QString{"%1 %2 %3"}.arg(address, 4, 16, QChar{'0'}).arg(opcode, -20, QChar{' '}).arg(name);
    }

    if (role == Qt::UserRole)
    {
        return false;
    }

    if (role == Qt::BackgroundRole)
    {
    }

    return {};
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index);
}

bool InstructionModel::setData(const QModelIndex& index, const QVariant& value, const int role)
{
    if (checkIndex(index) == false)
    {
        return false;
    }

    if (role == Qt::UserRole)
    {
        return false;
    }

    return QAbstractListModel::setData(index, value, role);
}