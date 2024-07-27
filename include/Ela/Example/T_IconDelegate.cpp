﻿#include "T_IconDelegate.h"

#include <QPainter>

#include "Def.h"
#include "ElaApplication.h"
T_IconDelegate::T_IconDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    connect(eApp, &ElaApplication::themeModeChanged, this, [=](ElaApplicationType::ThemeMode themeMode) { _themeMode = themeMode; });
}

T_IconDelegate::~T_IconDelegate()
{
}

void T_IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
    QStringList iconList = index.data(Qt::UserRole).toStringList();
    if (iconList.count() != 2)
    {
        return;
    }
    QString iconName = iconList.at(0);
    QString iconValue = iconList.at(1);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(22);
    painter->setFont(iconFont);
    if (_themeMode == ElaApplicationType::Light)
    {
        painter->setPen(Qt::black);
    }
    else
    {
        painter->setPen(Qt::white);
    }
    painter->drawText(option.rect.x() + option.rect.width() / 2 - 11, option.rect.y() + option.rect.height() / 2 - 11, iconValue);
    // 文字绘制
    QFont titlefont = painter->font();
    titlefont.setPointSize(10);
    painter->setFont(titlefont);
    qreal rowTextWidth = option.rect.width() * 0.8;
    int subTitleRow = painter->fontMetrics().horizontalAdvance(iconName) / rowTextWidth;
    if (subTitleRow > 0)
    {
        QString subTitleText = iconName;
        for (int i = 0; i < subTitleRow + 1; i++)
        {
            QString text = painter->fontMetrics().elidedText(subTitleText, Qt::ElideRight, rowTextWidth);
            if (text.right(3).contains("…"))
            {
                text = text.replace("…", subTitleText.mid(text.length() - 1, 1));
            }
            subTitleText.remove(0, text.length());
            painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(text) / 2, option.rect.y() + option.rect.height() - 10 * (subTitleRow + 1 - i), text);
        }
    }
    else
    {
        painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(iconName) / 2, option.rect.y() + option.rect.height() - 20, iconName);
    }
    painter->restore();
}
