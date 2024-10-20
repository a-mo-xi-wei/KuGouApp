//
// Created by WeiWang on 24-10-19.
//

#include "MyMenu.h"

#include <QCoreApplication>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QBoxLayout>
#include <qcoreevent.h>
#include <QToolButton>
#include <QWidgetAction>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))
constexpr int SHADOW_WIDTH = 5;
constexpr int RADIUS = 12;

MyMenu::MyMenu(const MenuKind &kind, QWidget *parent)
    : QMenu(parent)
      , m_playToolBtn(new QToolButton(this))
      , m_nextPlayToolBtn(new QToolButton(this))
      , m_addToToolBtn(new QToolButton(this)) {
    this->m_playToolBtn->installEventFilter(this);
    this->m_nextPlayToolBtn->installEventFilter(this);
    this->m_addToToolBtn->installEventFilter(this);
    initUi();
    switch (kind) {
        case MenuKind::SongOptionMenu: initSongOptionMenu();
            break;
        case MenuKind::SortOptionMenu: initSortOptionMenu();
            break;
        case MenuKind::None: break;
    }
}

void MyMenu::initUi() {
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setMouseTracking(true); {
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

void MyMenu::initSongOptionMenu() {
    this->setFixedSize(200, 450);
    //播放按钮
    auto a_playAction = new QWidgetAction(this);
    m_playToolBtn->setObjectName("playToolBtn");
    m_playToolBtn->setFixedSize(180, 35);
    m_playToolBtn->setIconSize(QSize(15, 15));
    m_playToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_playToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/play-gray.svg")));
    m_playToolBtn->setText(QStringLiteral("播放"));
    a_playAction->setDefaultWidget(m_playToolBtn);
    connect(a_playAction, &QWidgetAction::triggered, this, [this] {
        emit play();
    });
    connect(a_playAction, &QWidgetAction::hovered, this, [this] {
        //qDebug()<<"进入a_playToolBtn";
        checkHover();
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(m_playToolBtn, &enterEvent); // 发送事件
    });
    //下一首播放按钮
    auto a_nextPlayAction = new QWidgetAction(this);
    m_nextPlayToolBtn->setObjectName("nextPlayToolBtn");
    m_nextPlayToolBtn->setFixedSize(180, 35);
    m_nextPlayToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_nextPlayToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/next-play-gray.svg")));
    m_nextPlayToolBtn->setText(QStringLiteral("下一首播放"));
    a_nextPlayAction->setDefaultWidget(m_nextPlayToolBtn);
    connect(a_nextPlayAction, &QWidgetAction::triggered, this, [this] {
        emit nextPlay();
    });
    connect(a_nextPlayAction, &QWidgetAction::hovered, this, [this] {
        //qDebug()<<"进入a_nextPlayToolBtn";
        checkHover();
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(m_nextPlayToolBtn, &enterEvent); // 发送事件
    });
    //添加到
    auto a_addToAction = new QWidgetAction(this);
    m_addToToolBtn->setObjectName("addToToolBtn");
    m_addToToolBtn->setFixedSize(180, 35);
    m_addToToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_addToToolBtn->setIcon(QIcon(QStringLiteral("://Res/menuIcon/add-gray.svg")));
    m_addToToolBtn->setText(QStringLiteral("添加到"));
    a_addToAction->setDefaultWidget(m_addToToolBtn);
    connect(a_addToAction, &QWidgetAction::hovered, this, [this] {
        //qDebug()<<"进入a_addToToolBtn";
        checkHover();
        QEvent enterEvent(QEvent::Enter); // 创建进入事件
        QCoreApplication::sendEvent(m_addToToolBtn, &enterEvent); // 发送事件
    });
    this->addAction(a_playAction);
    this->addAction(a_nextPlayAction);
    this->addSeparator();
    this->addAction(a_addToAction);
}

void MyMenu::initSortOptionMenu() {
}

void MyMenu::checkHover() {
    if(this->m_lastHover) {
        QEvent leaveEvent(QEvent::Leave); // 创建进入事件
        QCoreApplication::sendEvent(m_lastHover, &leaveEvent); // 发送事件
    }
    this->m_currentHover = m_playToolBtn;
    this->m_lastHover = this->m_currentHover;
}

void MyMenu::paintEvent(QPaintEvent *event) {
    QMenu::paintEvent(event);
    //绘制阴影
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,
                        RADIUS, RADIUS);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2,
                            this->height() - (SHADOW_WIDTH - i) * 2, RADIUS, RADIUS);
        color.setAlpha(180 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void MyMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    this->setFocus(); // 强制widget接收焦点
}

bool MyMenu::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this->m_playToolBtn) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入，设置 hover 图标
            qDebug() << "进入m_playToolBtn";
            this->m_playToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/play-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，恢复默认图标
            qDebug() << "离开m_playToolBtn";
            this->m_playToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/play-gray.svg")));
        }
    } else if (watched == this->m_nextPlayToolBtn) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入，设置 hover 图标
            qDebug() << "进入m_nextPlayToolBtn";
            this->m_nextPlayToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/next-play-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，恢复默认图标
            qDebug() << "离开m_nextPlayToolBtn";
            this->m_nextPlayToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/next-play-gray.svg")));
        }
    } else if (watched == this->m_addToToolBtn) {
        if (event->type() == QEvent::Enter) {
            qDebug() << "进入m_addToToolBtn";
            this->m_addToToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/add-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            qDebug() << "离开m_addToToolBtn";
            this->m_addToToolBtn->setIcon(QIcon(QStringLiteral(":/Res/menuIcon/add-gray.svg")));
        }
    }
    return QMenu::eventFilter(watched, event);
}
