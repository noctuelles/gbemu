//
// Created by plouvel on 12/11/25.
//

#include "ui/debugger/InstructionModel.hxx"

#include "Common.hxx"

InstructionModel::InstructionModel(QObject* parent) : QAbstractListModel(parent) {}

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

    if (role == Qt::DisplayRole)
    {
        const auto& instruction{_instructions.at(index.row())};

        return QString{"%1:%2 %3"}
            .arg("UKNOWN")
            .arg(instruction.address, 4, 16, QChar{'0'})
            .toUpper()
            .arg(instruction.instruction);
    }
    if (role == Qt::UserRole)
    {
        return _instructions.at(index.row()).hasBreakpointDefined;
    }

    return {};
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index);
}

bool InstructionModel::setData(const QModelIndex& index, const QVariant& value, const int role)
{
    if (checkIndex(index) == false)
    {
        return false;
    }

    if (role == Qt::UserRole)
    {
        _instructions.at(index.row()).hasBreakpointDefined = value.toBool();
        return true;
    }

    return QAbstractListModel::setData(index, value, role);
}