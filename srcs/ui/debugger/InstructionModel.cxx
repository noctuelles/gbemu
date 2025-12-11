//
// Created by plouvel on 12/11/25.
//

#include "../../../includes/ui/debugger/InstructionModel.hxx"

InstructionModel::InstructionModel(QObject* parent) : QAbstractListModel(parent) {}

int InstructionModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int InstructionModel::rowCount(const QModelIndex& parent) const
{
    return 1500;
}

QVariant InstructionModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString{"Instruction %1"}.arg(index.row() + 1);
    }

    return {};
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index);
}