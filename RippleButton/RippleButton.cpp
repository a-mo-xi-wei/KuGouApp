#include "RippleButton.h"
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPainterPath>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QPointF>
#include <QTimer>
RippleButton::RippleButton(QWidget* parent)
    : QToolButton(parent),
    timer(new QTimer(this)),
    fill_color("#DDDDDD"),
    m_effect(std::make_unique<QGraphicsDropShadowEffect>(this))
{
    timer->setInterval(timeInterval); // 设置定时器时间间隔
    max_radius = qSqrt(width() * width() + height() * height()); // 计算最大半径
    //设置按钮的阴影效果
    m_effect->setOffset(0, 0); //阴影的偏移量（右，下）
    m_effect->setColor(QColor(0, 0, 0)); //阴影的颜色
    m_effect->setBlurRadius(6); //控制阴影的模糊程度（光源距离）
    this->setGraphicsEffect(this->m_effect.get());
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

void RippleButton::setMyIcon(const QIcon &ico) {
    this->m_ico = ico;
    this->setIcon(QIcon(this->m_ico));
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
        int iconSize = 12;
        this->m_ico.paint(&painter,(this->width() - iconSize) / 2, (this->height() - iconSize) / 2, iconSize, iconSize);
    }
}

void RippleButton::resizeEvent(QResizeEvent *event)
{
    QToolButton::resizeEvent(event);
    max_radius = qSqrt(width() * width() + height() * height()); // 重新计算最大半径
}
