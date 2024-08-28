#include "MusicItemWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QHBoxLayout>
#include<QVBoxLayout>


MusicItemWidget::MusicItemWidget(int index, const QString &name, QWidget *parent)
    :QWidget{parent}
    ,m_index(index)
{
    this->m_indexLab = new QLabel(QString::number(index),this);
    this->m_coverLab = new QLabel(this);
    this->m_nameLab = new QLabel(name,this);
    this->m_playToolBtn = new QToolButton(this);
    this->m_playNextToolBtn = new QToolButton(this);
    this->m_downloadToolBtn = new QToolButton(this);
    this->m_collectToolBtn = new QToolButton(this);
    this->m_moreToolBtn = new QToolButton(this);

    this->m_indexLab->setObjectName("indexLab");
    this->m_coverLab->setObjectName("coverLab");
    this->m_nameLab->setObjectName("nameLab");
    this->m_playToolBtn->setObjectName("playToolBtn");
    this->m_playNextToolBtn->setObjectName("playNextToolBtn");
    this->m_downloadToolBtn->setObjectName("downloadToolBtn");
    this->m_collectToolBtn->setObjectName("collectToolBtn");
    this->m_moreToolBtn->setObjectName("moreToolBtn");

    initUi();

    connect(this->m_playToolBtn,&QToolButton::clicked,this,[this]{emit playRequest(this->m_index);});
}

void MusicItemWidget::initUi()
{
    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_indexLab);
    hlayout->addWidget(this->m_coverLab);
    hlayout->addWidget(this->m_nameLab);
    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_playToolBtn);
    hlayout->addWidget(this->m_playNextToolBtn);
    hlayout->addWidget(this->m_downloadToolBtn);
    hlayout->addWidget(this->m_collectToolBtn);
    hlayout->addWidget(this->m_moreToolBtn);
}
