#include "RippleButton.h"
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QtMath>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>
RippleButton::RippleButton(QWidget* parent)
    : QToolButton(parent),
    timer(new QTimer(this)),
    fill_color("#DDDDDD")
{
    timer->setInterval(timeInterval); // 设置定时器时间间隔
    max_radius = qSqrt(width() * width() + height() * height()); // 计算最大半径
    setStyleSheet("*{border:none;}"); // 去除默认边框
}

void RippleButton::setSpeed(const int &timeinitval) {
    timer->setInterval(timeinitval);
}

void RippleButton::setFillColor(const QColor& fillcolor)
{
    fill_color = fillcolor;
}

void RippleButton::setRadius(int radius_)
{
    frame_radius = radius_;
}

void RippleButton::enterEvent(QEnterEvent* event)
{
    QToolButton::enterEvent(event);
    mouse_point = event->position(); // 记录鼠标进入坐标
    timer->disconnect(); // 断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发，半径增大
        radius += radius_var;
        if (radius > max_radius) {
            timer->stop(); // 达到最大半径，定时器停止
            return;
        }
        update(); // 调用绘制事件
    });
    timer->start(); // 定时器开始
}

void RippleButton::leaveEvent(QEvent* ev)
{
    QToolButton::leaveEvent(ev);
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=]{ // 定时器触发半径减小
        radius -= radius_var;
        if (radius < 0) {
            timer->stop(); // 减小到小于0时定时器停止
            radius = 0; // 确保半径不为负
            return;
        }
        update();
    });
    timer->start();
}

void RippleButton::paintEvent(QPaintEvent* event)
{
    QToolButton::paintEvent(event);
    if (!mouse_point.isNull() && radius > 0)
    {
        QPainter painter(this);
        QPainterPath path;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(fill_color));
        painter.setPen(Qt::NoPen);
        path.addRoundedRect(rect(), frame_radius, frame_radius);
        painter.setClipPath(path);
        painter.drawEllipse(mouse_point, radius, radius); // 画圆
    }
}

void RippleButton::resizeEvent(QResizeEvent *event)
{
    QToolButton::resizeEvent(event);
    max_radius = qSqrt(width() * width() + height() * height()); // 重新计算最大半径
}
