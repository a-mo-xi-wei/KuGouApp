//
// Created by WeiWang on 24-10-13.
//

#include "PlayWidget.h"
#include<QPainter>

PlayWidget::PlayWidget(QWidget *parent)
    : QWidget(parent) {
}

void PlayWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发送自定义的双击信号
    }
}

void PlayWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0, QColor("#eef2ff")); // 起始颜色
    gradient.setColorAt(1, QColor("#87CEFA")); // 结束颜色
    p.setBrush(gradient);
    p.drawRoundedRect(rect(), 8, 8);
}
