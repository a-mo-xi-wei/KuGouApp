#include "GLTabWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QStyleOption>
#include<QPainter>
#include<QPaintEvent>

GLTabWidget::GLTabWidget(QWidget *parent)
    : QWidget{parent}
{
    initUi();
    layoutUi();

}

void GLTabWidget::initUi()
{
    this->m_glLab = new QLabel("猜你喜欢",this);
    this->m_songNameLab = new QLabel("歌曲名称",this);
    this->m_playToolBtn = new QToolButton(this);
    this->m_nextToolBtn = new QToolButton(this);
    this->m_likeToolBtn = new QToolButton(this);
    this->m_dislikeToolBtn = new QToolButton(this);

    this->setObjectName("basic_window_widget");
    this->m_glLab->setObjectName("glLab");
    this->m_songNameLab ->setObjectName("songNameLab");
    this->m_playToolBtn->setObjectName("playToolBtn");
    this->m_nextToolBtn->setObjectName("nextToolBtn");
    this->m_likeToolBtn->setObjectName("likeToolBtn");
    this->m_dislikeToolBtn->setObjectName("dislikeToolBtn");
}

void GLTabWidget::paintEvent(QPaintEvent *ev)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void GLTabWidget::layoutUi()
{
    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Preferred,QSizePolicy::Preferred));
    hLayout1->addWidget(this->m_glLab);
    hLayout1->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Expanding,QSizePolicy::Preferred));

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Preferred,QSizePolicy::Preferred));
    hLayout2->addWidget(this->m_songNameLab);
    hLayout2->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Expanding,QSizePolicy::Preferred));

    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Preferred,QSizePolicy::Preferred));
    hLayout3->addWidget(this->m_playToolBtn);
    hLayout3->addWidget(this->m_nextToolBtn);
    hLayout3->addWidget(this->m_likeToolBtn);
    hLayout3->addWidget(this->m_dislikeToolBtn);
    hLayout3->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Expanding,QSizePolicy::Preferred));

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addSpacerItem(new QSpacerItem(40,20, QSizePolicy::Expanding));
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
}
