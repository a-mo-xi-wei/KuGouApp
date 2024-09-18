#include"SMaskWidget.h"
#include<QPainter>
#include<QPaintEvent>

SMaskWidget::SMaskWidget(QWidget *parent)
    :QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
}

void SMaskWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QColor color(0,0,0,160);
    // 设置绘制颜色，带有透明度
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(),8,8);

}

