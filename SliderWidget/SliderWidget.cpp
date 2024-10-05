#include "SliderWidget.h"
#include<QMouseEvent>

SliderWidget::SliderWidget(QWidget *parent,Qt::Orientation orientation)
    : QSlider(orientation, parent)
    ,m_currentValue(20)
{
    this->setCursor(Qt::PointingHandCursor);
}

void SliderWidget::mousePressEvent(QMouseEvent *event) {
    // 获取鼠标点击相对于滑动条的位置
    int pos = orientation() == Qt::Vertical ? event->position().y() : event->position().x();
    // 获取滑动条的可用长度 (即滑块可以滑动的范围)
    int sliderLength = this->orientation() == Qt::Vertical ? this->height() : this->width();
    // 计算点击位置相对于整个滑动条的百分比
    double percentage = static_cast<double>(pos) / sliderLength;
    if(this->orientation() == Qt::Vertical)percentage = 1 - percentage;
    // 根据百分比计算出滑动条的新值
    this->m_currentValue = this->m_minValue + percentage * (this->m_maxValue - this->m_minValue);
    // 设置滑动条的新值
    this->setValue(this->m_currentValue);
    // 正在按下
    this->m_isPressing = true;
    // 让父类处理其余的鼠标事件
    QSlider::mousePressEvent(event);
}

void SliderWidget::mouseMoveEvent(QMouseEvent *event) {
    if(this->m_isPressing) {
        // 获取鼠标移动相对于滑动条的位置
        int pos = orientation() == Qt::Vertical ? event->position().y() : event->position().x();
        // 获取滑动条的可用长度
        int sliderLength = this->orientation() == Qt::Vertical ? this->height() : this->width();
        // 将鼠标位置转换为滑动条值
        double percentage = static_cast<double>(pos) / sliderLength;
        if(this->orientation() == Qt::Vertical)percentage = 1 - percentage;
        // 根据百分比计算出滑动条的新值
        this->m_currentValue = this->m_minValue + percentage * (this->m_maxValue - this->m_minValue);
        // 设置滑动条的新值
        this->setValue(this->m_currentValue);
    }
    QSlider::mouseMoveEvent(event);
}

void SliderWidget::mouseReleaseEvent(QMouseEvent *event) {
    // 鼠标释放时，重置按下标志
    m_isPressing = false;
    QSlider::mouseReleaseEvent(event);
}

void SliderWidget::showEvent(QShowEvent *event) {
    QSlider::showEvent(event);
    // 获取滑动条的最小值和最大值
    this->m_minValue = this->minimum();
    this->m_maxValue = this->maximum();
    this->m_parent = qobject_cast<MyWidget*>(this->parent());
    // 确保 m_parent 不为空再进行连接
    if (this->m_parent)
        connect(this->m_parent,&MyWidget::noVolume,this,[this](bool flag){onNoVolume(flag);});
}

void SliderWidget::onNoVolume(bool flag) {
    if(flag) {
        this->setValue(0);
    }
    else {
        this->setValue(this->m_currentValue);
    }
}
