//
// Created by WeiWang on 24-10-19.
//

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include <QEvent>
#include <QMenu>

#include <QToolButton>
class MenuBtn : public QToolButton
{
    Q_OBJECT
public:
    explicit MenuBtn( QWidget *parent = nullptr):QToolButton(parent) {
        this->setIconSize(QSize(15, 15));
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        this->installEventFilter(this);
    }
    void initIcon(const QIcon& icon1,const QIcon& icon2) {
        this->m_icon1 = icon1;
        this->m_icon2 = icon2;
    }
protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::Enter) {
            //qDebug()<<"进入";
            this->setIcon(m_icon2);
        } else if (event->type() == QEvent::Leave) {
            //qDebug()<<"离开";
            this->setIcon(m_icon1);
        }
        return QToolButton::eventFilter(watched, event);
    }
private:
    QIcon m_icon1;//leave
    QIcon m_icon2;//enter
};


class MyMenu : public QMenu
{
    Q_OBJECT

public:
    enum MenuKind {
        SongOptionMenu,
        SortOptionMenu,
        None
    };
    explicit MyMenu(const MenuKind& kind, QWidget *parent = nullptr);

    void initUi();

    void initSongOptionMenu();

    void initSortOptionMenu();

    void checkHover();

signals:
    void play();

    void nextPlay();

    void download();

    void share();

protected:
    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    //为了解决QWidgetAction没有与hovered相反的离开事件设置下面两个变量
    QVector<QWidget*> m_lastHover{};
    QVector<QWidget*> m_currentHover{};
};


#endif //MYWIDGETMENU_H
