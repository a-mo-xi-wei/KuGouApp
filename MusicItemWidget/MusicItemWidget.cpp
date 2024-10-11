#include "MusicItemWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>

MusicItemWidget::MusicItemWidget(const SongInfor& infor, QWidget *parent)
    :QWidget(parent)
    ,m_information(infor)
{
    this->m_index           = m_information.index;
    this->m_name            = m_information.songName;
    this->m_duration        = m_information.duration;
    this->m_cover           = m_information.cover;
    this->m_singer          = m_information.signer;
    qDebug()<<"m_index: "<<m_index<<" name: "<<m_name<<" duration: "<<m_duration<<
        " cover: "<<m_cover<<"m_singer: "<<m_singer;
    this->m_indexLab        = new QLabel(QString("%1").arg(this->m_index, 2, 10, QChar('0')), this);
    this->m_coverLab        = new QLabel(this);
    this->m_coverLab->setAlignment(Qt::AlignCenter);
    this->m_coverLab->setFixedSize(45,45);
    this->m_coverLab->setPixmap(this->m_cover.scaled(this->m_coverLab->size()));
    this->m_nameLab         = new QLabel(this->m_name,this);
    this->m_singerLab       = new QLabel(this->m_singer,this);
    this->m_spacerItem1     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_spacerItem2     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_durationLab     = new QLabel(this->m_duration,this);
    this->m_playToolBtn     = new QToolButton(this);
    this->m_playNextToolBtn = new QToolButton(this);
    this->m_downloadToolBtn = new QToolButton(this);
    this->m_collectToolBtn  = new QToolButton(this);
    this->m_moreToolBtn     = new QToolButton(this);

    this->m_indexLab->setObjectName("indexLab");
    this->m_coverLab->setObjectName("coverLab");
    this->m_nameLab->setObjectName("nameLab");
    this->m_singerLab->setObjectName("singerLab");
    this->m_playToolBtn->setObjectName("playToolBtn");
    this->m_playNextToolBtn->setObjectName("playNextToolBtn");
    this->m_downloadToolBtn->setObjectName("downloadToolBtn");
    this->m_collectToolBtn->setObjectName("collectToolBtn");
    this->m_moreToolBtn->setObjectName("moreToolBtn");

    initUi();

    connect(this->m_playToolBtn,&QToolButton::clicked,this,
        [this]{emit playRequest(this->m_index);});
}

void MusicItemWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);

}

void MusicItemWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);

}

void MusicItemWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void MusicItemWidget::initUi()
{
    this->m_playToolBtn->setIcon(QIcon("://Res/tabIcon/play3-gray.svg"));
    this->m_playNextToolBtn->setIcon(QIcon("://Res/tabIcon/add-music-list-gray.svg"));
    this->m_downloadToolBtn->setIcon(QIcon("://Res/window/download.svg"));
    this->m_collectToolBtn->setIcon(QIcon("://Res/window/collect.svg"));
    this->m_moreToolBtn->setIcon(QIcon("://Res/tabIcon/more2-gray.svg"));

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_indexLab);
    hlayout->addWidget(this->m_coverLab);
    QVBoxLayout* vlayout = new QVBoxLayout;
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
}
