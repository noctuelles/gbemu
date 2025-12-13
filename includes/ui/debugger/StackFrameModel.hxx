//
// Created by plouvel on 12/13/25.
//

#ifndef GBEMU_STACKFRAMEMODEL_HXX
#define GBEMU_STACKFRAMEMODEL_HXX
#include "Common.hxx"
#include "ui/debugger/InstructionListView.hxx"

class StackFrameModel final : public QAbstractListModel
{
  public:
    explicit StackFrameModel(QObject* parent = nullptr);

    void updateStack(uint16_t sp, const AddressSpace& addressSpace);

    int      columnCount(const QModelIndex& parent) const override;
    int      rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

  private:
    static constexpr uint8_t STACK_SIZE = 15;

    QVector<QPair<uint16_t, uint16_t>> _stack;
};

#endif  // GBEMU_STACKFRAMEMODEL_HXX
