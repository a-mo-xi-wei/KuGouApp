//
// Created by WeiWang on 24-10-16.
//

#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QScrollArea>


class MyScrollArea : public QScrollArea {
Q_OBJECT

public:
    explicit MyScrollArea(QWidget *parent = nullptr);
    void setAnimating(const bool& animating);
protected:
    void wheelEvent(QWheelEvent* event) override;
private:
    bool m_isAnimating;  // 标记动画是否正在进行
};


#endif //MYSCROLLAREA_H
