#include "MusicItemWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpacerItem>

MusicItemWidget::MusicItemWidget(const QMap<QString,QString>& inforMap, QWidget *parent)
    :QWidget(parent)
    ,m_informationMap(inforMap)
{
    this->m_index           = inforMap.value("index").toInt();
    this->m_name            = inforMap.value("name");
    this->m_duration        = inforMap.value("duration");
    this->m_coverPath       = inforMap.value("cover");
    this->m_singer          = inforMap.value("singer");

    this->m_indexLab        = new QLabel(QString("%1").arg(this->m_index, 2, 10, QChar('0')), this);
    this->m_coverLab        = new QLabel(this);
    this->m_nameLab         = new QLabel(this->m_name,this);
    this->m_singerLab         = new QLabel(this->m_singer,this);
    this->m_spacerItem1     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_spacerItem2     = new QSpacerItem(40,20,QSizePolicy::Expanding);
    this->m_durationLab     = new QLabel(this);
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
