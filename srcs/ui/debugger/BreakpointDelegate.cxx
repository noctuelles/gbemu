//
// Created by plouvel on 12/4/25.
//

#include "../../../includes/ui/debugger/BreakpointDelegate.hxx"

#include <QPainter>

#include "../../../includes/ui/debugger/InstructionListView.hxx"

QSize BreakpointDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& idx) const
{
    QSize s = QStyledItemDelegate::sizeHint(opt, idx);
    s.rwidth() += GUTTER_WIDTH;
    return s;
}

void BreakpointDelegate::paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& idx) const
{
    QStyleOptionViewItem option(opt);
    auto*                view{qobject_cast<const InstructionListView*>(option.widget)};
    const QRect          original{option.rect};

    initStyleOption(&option, idx);
    option.rect.adjust(GUTTER_WIDTH, 0, 0, 0);

    QStyledItemDelegate::paint(p, option, idx);

    if (view == nullptr)
    {
        return;
    }

    const bool  isHovered = (idx == view->hoveredIndex()) && view->hoveredX() < GUTTER_WIDTH;
    const bool  bp{false};
    const int   diameter{qMin(option.rect.height() - 6, GUTTER_WIDTH - 6)};
    const int   cx{original.left() + (GUTTER_WIDTH - diameter) / 2};
    const int   cy{original.center().y() - diameter / 2};
    const QRect circleRect{cx, cy + 1, diameter, diameter};

    p->save();
    p->setRenderHint(QPainter::Antialiasing);

    if (bp)
    {
        p->setBrush(Qt::red);
        p->setPen(Qt::NoPen);
        p->drawEllipse(circleRect);
    }
    else if (isHovered)
    {
        constexpr auto faded{QColor(255, 0, 0, 80)};

        p->setBrush(faded);
        p->setPen(Qt::NoPen);
        p->drawEllipse(circleRect);
    }

    p->restore();
}
