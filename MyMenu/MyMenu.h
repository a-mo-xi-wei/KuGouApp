//
// Created by WeiWang on 24-10-19.
//

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include <QMenu>
#include <QFile>

class QGraphicsDropShadowEffect;

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    explicit MyMenu(QWidget *parent = nullptr);

    void initMenu();

    void initUi();

protected:
    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent *event) override;

};


#endif //MYWIDGETMENU_H
