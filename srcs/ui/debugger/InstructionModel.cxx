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

    SM83::Disassembler disassembler{addressSpace};
    const auto         result{disassembler.disassemble(0x0000, 0x1000, std::nullopt)};

    _instructions.clear();

    for (const auto& [addrAndBytecode, inst] : result)
    {
        QVector<uint8_t> vect{addrAndBytecode.second.begin(), addrAndBytecode.second.end()};

        _instructions.push_back({inst.c_str(), addrAndBytecode.first, std::move(vect), false});
    }

    endResetModel();
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
    const auto& instruction{_instructions.at(index.row())};

    if (role == Qt::DisplayRole)
    {
        QString byteCode{};

        for (const auto byte : instruction.bytes)
        {
            byteCode += QString{"%1 "}.arg(byte, 2, 16, QChar{'0'}).toUpper();
        }

        return QString{"%1 %2 %3"}
            .arg(instruction.address, 4, 16, QChar{'0'})
            .arg(byteCode, -20, QChar{' '})
            .arg(instruction.instruction.toLower());
    }

    if (role == Qt::UserRole)
    {
        return _instructions.at(index.row()).hasBreakpointDefined;
    }

    if (role == Qt::BackgroundRole)
    {
        if (instruction.hasBreakpointDefined)
        {
            return QColor{0xFF, 0x00, 0x00, 0xFF};
        }
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
        _instructions[index.row()].hasBreakpointDefined = value.toBool();
        return true;
    }

    return QAbstractListModel::setData(index, value, role);
}