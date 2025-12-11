//
// Created by plouvel on 12/4/25.
//

#ifndef GBEMU_BREAKPOINTDELEGATE_HXX
#define GBEMU_BREAKPOINTDELEGATE_HXX

#include <QStyledItemDelegate>

class BreakpointDelegate final : public QStyledItemDelegate
{
    Q_OBJECT
  public:
    static constexpr int GUTTER_WIDTH = 20;

    using QStyledItemDelegate::QStyledItemDelegate;

    bool                editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;
    void                paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;
};

#endif  // GBEMU_BREAKPOINTDELEGATE_HXX
