//
// Created by plouvel on 12/13/25.
//

#include "ui/debugger/StackFrameModel.hxx"

#include "Utils.hxx"

StackFrameModel::StackFrameModel(QObject* parent) : QAbstractListModel(parent)
{
    _stack.reserve(STACK_SIZE);
}

void StackFrameModel::updateStack(uint16_t sp, const AddressSpace& addressSpace)
{
    beginResetModel();

    _stack.clear();
    for (size_t i = 0; i < STACK_SIZE; ++i)
    {
        const auto address{static_cast<uint32_t>(sp + i * 2)};

        if (address > 0xFFFF)
        {
            break;
        }

        _stack.emplace_back(address, Utils::toWord(addressSpace[address + 1], addressSpace[address]));
    }

    endResetModel();
}

int StackFrameModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int StackFrameModel::rowCount(const QModelIndex& parent) const
{
    return _stack.size();
}

QVariant StackFrameModel::data(const QModelIndex& index, int role) const
{
    const auto& [address, value]{_stack.at(index.row())};

    if (role == Qt::DisplayRole)
    {
        return QString{"$%1: $%2"}.arg(address, 4, 16, QChar{'0'}).arg(value, 4, 16, QChar{'0'}).toUpper();
    }

    return QVariant{};
}