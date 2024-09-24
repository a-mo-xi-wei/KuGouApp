#include"SMaskWidget.h"

#include <QMouseEvent>
#include<QPainterPath>

SMaskWidget::SMaskWidget(QWidget *parent)
    : QWidget(parent),m_radius(0),m_centerX(0),m_centerY(0),m_w(0),m_h(0){
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
}

void SMaskWidget::calOnce() {
    static bool executed = false; // 静态变量
    m_w = static_cast<const float>(rect().width());
    m_h = static_cast<const float>(rect().height());
    m_radius = static_cast<float>(rect().width())/5;
    m_centerX = static_cast<const float>(rect().width())/2;
    m_centerY = static_cast<const float>(rect().height())/2;
    m_ap = static_cast<const QPointF>(QPointF(m_centerX - m_w / 26, m_centerY - m_h / 18));
    m_bp = static_cast<const QPointF>(QPointF(m_centerX - m_w / 26, m_centerY + m_h / 18));
    m_cp = static_cast<const QPointF>(QPointF(m_centerX + m_w / 15, m_centerY));
}

bool SMaskWidget::isMouseInCircle(const float &mouseX, const float &mouseY) {
    // 计算鼠标位置和圆心的距离
    const float dx = mouseX - m_w/2;
    const float dy = mouseY - m_h/2;
    return (dx * dx + dy * dy <= m_radius * m_radius); // 在圆内时返回 true
}

void SMaskWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    calOnce();
    constexpr QColor color(0, 0, 0, 160);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置绘制颜色，带有透明度
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 8, 8);
    if(this->m_isEnter) {
        // 创建一个绘制路径
        QPainterPath path;
        // 定义三角形的三个顶点
        path.moveTo(m_ap.x(),m_ap.y());
        path.lineTo(m_bp.x(),m_bp.y());
        path.lineTo(m_cp.x(),m_cp.y());
        path.closeSubpath(); // 闭合路径
        path = path.simplified();
        // 使用路径组合来创建镂空效果
        QPainterPath circlePath;
        circlePath.addEllipse(QPointF(m_centerX, m_centerY), m_radius - 1, m_radius - 1);
        // 使用路径组合来创建镂空效果
        QPainterPath combinedPath = circlePath.subtracted(path);
        combinedPath = combinedPath.simplified();
        // 绘制镂空效果
        painter.setBrush(Qt::white); // 设置填充颜色为白色
        painter.drawPath(combinedPath);
    }
    else {
        //绘制圆形
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(m_centerX,m_centerY), m_radius,m_radius);
        // 创建一个绘制路径
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        QPainterPath path;
        // 定义三角形的三个顶点
        path.moveTo(m_ap.x(),m_ap.y());
        path.lineTo(m_bp.x(),m_bp.y());
        path.lineTo(m_cp.x(),m_cp.y());
        path.closeSubpath(); // 闭合路径
        path = path.simplified();
        painter.drawPath(path);
    }
}
void SMaskWidget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    auto x = static_cast<float>(event->pos().x());
    auto y= static_cast<float>(event->pos().y());
    this->m_isEnter = isMouseInCircle(x,y);
    if(this->m_isEnter)this->setCursor(Qt::PointingHandCursor);
    else this->setCursor(Qt::ArrowCursor);
    update();
}

void SMaskWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    update();
}

void SMaskWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    update();
}
