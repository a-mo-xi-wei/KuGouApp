//
// Created by WeiWang on 24-10-19.
//

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include <QMenu>

class QToolButton;

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

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    //为了解决QWidgetAction没有与hovered相反的离开事件设置下面两个变量
    QToolButton* m_lastHover{};
    QToolButton* m_currentHover{};

    QToolButton* m_playToolBtn{};
    QToolButton* m_nextPlayToolBtn{};
    QToolButton* m_addToToolBtn{};
};


#endif //MYWIDGETMENU_H
