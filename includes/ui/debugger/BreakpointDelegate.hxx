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
    using QStyledItemDelegate::QStyledItemDelegate;

    QSize sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;

    void paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const override;

  private:
    static constexpr int GUTTER_WIDTH = 20;
};

#endif  // GBEMU_BREAKPOINTDELEGATE_HXX
