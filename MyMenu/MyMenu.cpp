//
// Created by WeiWang on 24-10-19.
//

#include "MyMenu.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include<QVBoxLayout>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))
constexpr int SHADOW_WIDTH = 5;
constexpr int RADIUS = 12;
MyMenu::MyMenu(QWidget *parent)
    : QMenu(parent)
{
    initUi();
    //initMenu();

}

void MyMenu::initMenu() {
    this->addAction(new QAction("Action 1"));
    this->addAction(new QAction("Action 2"));
    this->addAction(new QAction("Action 3"));
}

void MyMenu::initUi() {
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint  | Qt::NoDropShadowWindowHint);

    this->setFixedSize(200,450);
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/menu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

void MyMenu::paintEvent(QPaintEvent *event) {
    QMenu::paintEvent(event);
    //绘制阴影
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(SHADOW_WIDTH,SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,RADIUS,RADIUS);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH- i, this->width() - (SHADOW_WIDTH- i) * 2, this->height() - (SHADOW_WIDTH- i) * 2, RADIUS, RADIUS);
        color.setAlpha(180 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(color);
        painter.drawPath(path);
    }
}


void MyMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    this->setFocus();  // 强制widget接收焦点
}
