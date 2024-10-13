//
// Created by WeiWang on 24-9-18.
//

#include "CenterWidget.h"
#include<QPainter>

CenterWidget::CenterWidget(QWidget *parent)
    : QWidget(parent) {
}

void CenterWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
    gradient.setColorAt(0, QColor("#eef2ff")); // 起始颜色
    gradient.setColorAt(0.25, QColor("#dff1ff")); // 中间颜色
    gradient.setColorAt(0.5, QColor("#e8ffd4")); // 中间颜色
    gradient.setColorAt(0.75, QColor("#dff1ff")); // 中间颜色
    gradient.setColorAt(1, QColor("#eef2ff")); // 结束颜色
    p.setBrush(gradient);
    p.drawRoundedRect(rect(), 8, 8);
}
