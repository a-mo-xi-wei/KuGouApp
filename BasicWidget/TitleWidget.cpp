//
// Created by WeiWang on 24-9-18.
//

#include "TitleWidget.h"
#include<QPainter>

TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent) {
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发送自定义的双击信号
    }
}

void TitleWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#87CEFA"))); // 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); // 结束颜色
    p.setBrush(gradient);
    p.drawRoundedRect(rect(), 8, 8);
}
