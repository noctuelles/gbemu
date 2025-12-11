//
// Created by plouvel on 12/4/25.
//

#include "ui/debugger/BreakpointDelegate.hxx"

#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

#include "ui/debugger/InstructionListView.hxx"

bool BreakpointDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                     const QModelIndex& index)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        const auto* me{dynamic_cast<QMouseEvent*>(event)};

        if (me == nullptr)
        {
            return false;
        }

        if (me->pos().x() < GUTTER_WIDTH && me->button() == Qt::LeftButton)
        {
            const auto isToggled{model->data(index, Qt::UserRole).toBool()};
            model->setData(index, !isToggled, Qt::UserRole);
            return true;
        }
        if (me->pos().x() < GUTTER_WIDTH && me->button() == Qt::RightButton)
        {
            if (model->data(index, Qt::UserRole).toBool())
            {
                QMenu menu{};

                menu.addAction("Set condition...");
                menu.exec(me->globalPos());

                return true;
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

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
    const bool  isBreakpointSet{idx.data(Qt::UserRole).toBool()};
    const int   diameter{qMin(option.rect.height() - 6, GUTTER_WIDTH - 6)};
    const int   cx{original.left() + (GUTTER_WIDTH - diameter) / 2};
    const int   cy{original.center().y() - diameter / 2};
    const QRect circleRect{cx, cy + 1, diameter, diameter};

    p->save();
    p->setRenderHint(QPainter::Antialiasing);

    if (isBreakpointSet)
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
