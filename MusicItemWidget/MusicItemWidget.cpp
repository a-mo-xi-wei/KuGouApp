#include "MusicItemWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QVBoxLayout>
#include<QSpacerItem>
#include<QFile>
#include<QTimer>
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QCoreApplication>
#include <QPainterPath>
#include <QtMath>
#include <QPointF>
// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\")))
QPixmap roundedPix(const QPixmap &src, QSize size, int radius) {
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}
MusicItemWidget::MusicItemWidget(const SongInfor& infor, QWidget *parent)
    :QFrame(parent)
    ,m_information(infor)
    ,timer(new QTimer(this))
{

    this->m_index           = m_information.index;
    this->m_name            = m_information.songName;
    this->m_duration        = m_information.duration;
    this->m_cover           = m_information.cover;
    this->m_singer          = m_information.signer;
    //qDebug()<<"m_index: "<<m_index<<" name: "<<m_name<<" duration: "<<m_duration<<
    //    " cover: "<<m_cover<<"m_singer: "<<m_singer;
    this->m_indexLab        = new QLabel(QString("%1").arg(this->m_index, 2, 10, QChar('0')), this);
    this->m_coverLab        = new QLabel(this);
    this->m_coverLab->setAlignment(Qt::AlignCenter);
    this->m_coverLab->setFixedSize(45,45);
    this->m_coverLab->setPixmap(roundedPix(this->m_cover,this->m_coverLab->size(),8));
    this->m_nameLab         = new QLabel(this->m_name,this);
    this->m_singerLab       = new QLabel(this->m_singer,this);
    this->m_spacerItem1     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_spacerItem2     = new QSpacerItem(40,20,QSizePolicy::Fixed);
    this->m_durationLab     = new QLabel(this->m_duration,this);
    this->m_playToolBtn     = new QToolButton(this);
    this->m_playNextToolBtn = new QToolButton(this);
    this->m_downloadToolBtn = new QToolButton(this);
    this->m_collectToolBtn  = new QToolButton(this);
    this->m_moreToolBtn     = new QToolButton(this);

    this->setObjectName("window");
    this->m_indexLab->setObjectName("indexLab");
    this->m_coverLab->setObjectName("coverLab");
    this->m_nameLab->setObjectName("nameLab");
    this->m_singerLab->setObjectName("singerLab");
    this->m_playToolBtn->setObjectName("playToolBtn");
    this->m_playNextToolBtn->setObjectName("playNextToolBtn");
    this->m_downloadToolBtn->setObjectName("downloadToolBtn");
    this->m_collectToolBtn->setObjectName("collectToolBtn");
    this->m_moreToolBtn->setObjectName("moreToolBtn");
    //设置样式
    QFile file((GET_CURRENT_DIR + "/item.css").data());
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }

    timer->setInterval(timeInterval); // 设置定时器时间间隔
    max_radius = qSqrt(width() * width() + height() * height()); // 计算最大半径

    initUi();

    connect(this->m_playToolBtn,&QToolButton::clicked,this,[this]{emit playRequest();});
}

void MusicItemWidget::setInitval(const int &timeinitval) {
    timer->setInterval(timeinitval);
}

void MusicItemWidget::setFillColor(const QColor &fillcolor) {
    fill_color = fillcolor;
}

void MusicItemWidget::setRadius(int radius_) {
    frame_radius = radius_;
}

void MusicItemWidget::setInformation(const SongInfor &info) {
    this->m_index = info.index;
    this->m_name = info.songName;
    this->m_duration = info.duration;
    this->m_cover = info.cover;
    this->m_singer = info.signer;
    this->m_indexLab->setText(QString("%1").arg(this->m_index, 2, 10, QChar('0')));
    this->m_coverLab->setPixmap(roundedPix(this->m_cover, this->m_coverLab->size(), 8));
    this->m_nameLab->setText(this->m_name);
    this->m_singerLab->setText(this->m_singer);
    this->m_durationLab->setText(this->m_duration);
    update(); // 重绘
}

void MusicItemWidget::setPlayState(const bool &state) {
    this->m_isPlaying = state;
    if(this->m_isPlaying) {
        //发送进入事件
        // 创建一个进入事件
        QEvent enterEvent(QEvent::Enter);
        // 发送离开事件
        QCoreApplication::sendEvent(this, &enterEvent);
    }
    else {
        //发送离开事件
        // 创建一个离开事件
        QEvent leaveEvent(QEvent::Leave);
        // 发送离开事件
        QCoreApplication::sendEvent(this, &leaveEvent);
    }
}

void MusicItemWidget::enterEvent(QEnterEvent *event) {
    QFrame::enterEvent(event);

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

void MusicItemWidget::leaveEvent(QEvent *event) {
    QFrame::leaveEvent(event);
    if(!this->m_isPlaying) {
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
}

void MusicItemWidget::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
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

void MusicItemWidget::resizeEvent(QResizeEvent *event) {
    QFrame::resizeEvent(event);
    max_radius = qSqrt(width() * width() + height() * height()); // 重新计算最大半径
    //设置弹簧宽度
    this->m_spacerItem2->changeSize(this->width()/5,20,QSizePolicy::Fixed);
}

void MusicItemWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QFrame::mouseDoubleClickEvent(event);
    this->m_playToolBtn->clicked();
}

void MusicItemWidget::initUi()
{
    this->m_playToolBtn     ->setIcon(QIcon(QStringLiteral("://Res/tabIcon/play3-gray.svg")));
    this->m_playNextToolBtn ->setIcon(QIcon(QStringLiteral("://Res/tabIcon/add-music-list-gray.svg")));
    this->m_downloadToolBtn ->setIcon(QIcon(QStringLiteral("://Res/window/download.svg")));
    this->m_collectToolBtn  ->setIcon(QIcon(QStringLiteral("://Res/window/collect.svg")));
    this->m_moreToolBtn     ->setIcon(QIcon(QStringLiteral("://Res/tabIcon/more2-gray.svg")));

    this->m_playToolBtn     ->setCursor(Qt::PointingHandCursor);
    this->m_playNextToolBtn ->setCursor(Qt::PointingHandCursor);
    this->m_downloadToolBtn ->setCursor(Qt::PointingHandCursor);
    this->m_collectToolBtn  ->setCursor(Qt::PointingHandCursor);
    this->m_moreToolBtn     ->setCursor(Qt::PointingHandCursor);

    auto hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_indexLab);
    hlayout->addWidget(this->m_coverLab);
    auto vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_singerLab);
    hlayout->addLayout(vlayout);
    hlayout->addSpacerItem(this->m_spacerItem1);
    hlayout->addWidget(this->m_durationLab);
    hlayout->addSpacerItem(this->m_spacerItem2);
    hlayout->addWidget(this->m_playToolBtn);
    hlayout->addWidget(this->m_playNextToolBtn);
    hlayout->addWidget(this->m_downloadToolBtn);
    hlayout->addWidget(this->m_collectToolBtn);
    hlayout->addWidget(this->m_moreToolBtn);

    this->m_durationLab->move(this->width()*5/6,(this->height() - this->m_durationLab->height()) / 2);
}
