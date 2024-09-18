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

void TitleWidget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);

    p.setPen(Qt::NoPen);

    p.setBrush(QColor("#e2f1ff"));

    p.drawRoundedRect(rect(),10,10);

}
