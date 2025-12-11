//
// Created by plouvel on 12/11/25.
//

#include "ui/RegisterModel.hxx"

RegisterModel::RegisterModel(QObject* parent) : QAbstractTableModel(parent) {}

void RegisterModel::setRegisters(const std::vector<RegisterEntry>& registers)
{
    beginResetModel();
    _registers = registers;
    endResetModel();
}

QVariant RegisterModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return QString{"Value"};
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        return QString{_registers.at(section).name};
    }

    return {};
}

int RegisterModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(_registers.size());
}

int RegisterModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant RegisterModel::data(const QModelIndex& index, const int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    const auto& registerEntry{_registers.at(index.row())};

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return QString{"$%1"}
                .arg(registerEntry.value, std::to_underlying(registerEntry.bitSize) / 4, 16, QChar{'0'})
                .toUpper();
        }
    }

    return {};
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags f{Qt::ItemIsSelectable | Qt::ItemIsEnabled};

    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    if (index.column() == 0 && _registers.at(index.row()).editable)
    {
        f |= Qt::ItemIsEditable;
    }

    return f | QAbstractTableModel::flags(index);
}

bool RegisterModel::setData(const QModelIndex& index, const QVariant& value, const int role)
{
    if (!index.isValid())
    {
        return false;
    }

    if (role == Qt::EditRole)
    {
        auto&            registerEntry{_registers.at(index.row())};
        qulonglong       newValue{};
        const qulonglong mask{(1U << std::to_underlying(registerEntry.bitSize)) - 1U};
        bool             ok{};

        if (!registerEntry.editable)
        {
            return false;
        }

        newValue = value.toString().toULongLong(&ok, 16);
        if (!ok)
        {
            return false;
        }
        newValue &= mask;

        registerEntry.value = newValue;

        emit dataChanged(index, index);

        return true;
    }

    return QAbstractTableModel::setData(index, value, role);
}