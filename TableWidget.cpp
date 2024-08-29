#include "TableWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QPaintEvent>
#include<QEnterEvent>
#include<QEvent>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QSpacerItem>
#include<QStyleOption>
#include<QPainter>
#include<QGridLayout>

TableWidget::TableWidget(const QString &title, QWidget *parent)
    :QWidget(parent)
{
    this->m_titleLab        = new QLabel(title,this);
    this->m_play_ToolBtn    = new QToolButton(this);
    this->m_adjust_ToolBtn  = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_ToolBtn    = new QToolButton(this);
    this->m_tabWidget       = new QWidget;

    initUi();

    this->m_adjust_ToolBtn->hide();
}

void TableWidget::paintEvent(QPaintEvent *ev)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void TableWidget::enterEvent(QEnterEvent *ev)
{
    this->m_adjust_ToolBtn->show();
}

void TableWidget::leaveEvent(QEvent *ev)
{
    this->m_adjust_ToolBtn->hide();
}

void TableWidget::initUi()
{
    this->m_titleLab->setObjectName("titleLab");
    this->m_play_ToolBtn->setObjectName("play_ToolBtn");
    this->m_adjust_ToolBtn->setObjectName("adjust_ToolBtn");
    this->m_refresh_ToolBtn->setObjectName("refresh_ToolBtn");
    this->m_more_ToolBtn->setObjectName("more_ToolBtn");
    this->m_tabWidget->setObjectName("tabWidget");

    this->m_play_ToolBtn->setIcon(QIcon("://image/tabIcon/play2-gray.svg"));
    this->m_adjust_ToolBtn->setIcon(QIcon("://image/tabIcon/adjust-column-gray.svg"));
    this->m_refresh_ToolBtn->setIcon(QIcon("://image/tabIcon/refresh-gray.svg"));
    this->m_more_ToolBtn->setIcon(QIcon("://image/tabIcon/more2-gray.svg"));

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->setStyleSheet("QLabel#titleLab{font-size:20px;}QToolButton{background-color:rgba(255,255,255,0);}");
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(this->m_titleLab);
    hlayout->addWidget(this->m_play_ToolBtn);
    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_adjust_ToolBtn);
    hlayout->addWidget(this->m_refresh_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);

    QGridLayout* glayout = new QGridLayout(this->m_tabWidget);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto item = new ItemListWidget(QPixmap("://image/tabIcon/music-cover.jpg"),"歌曲名字","作者",this->m_tabWidget);
            glayout->addWidget(item,i,j);
        }
    }

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(this->m_tabWidget);
}



ItemListWidget::ItemListWidget(const QPixmap &coverPix, const QString &name, const QString &author, QWidget *parent)
    :QWidget(parent)
{
    this->m_coverLab     = new QLabel(this);
    this->m_nameLab      = new QLabel(name,this);
    this->m_authorLab    = new QLabel(author,this);
    this->m_play_ToolBtn = new QToolButton(this);
    this->m_like_ToolBtn = new QToolButton(this);
    this->m_more_ToolBtn = new QToolButton(this);

    this->m_coverLab->setObjectName("coverLab");
    this->m_nameLab->setObjectName("nameLab");
    this->m_authorLab->setObjectName("authorLab");
    this->m_play_ToolBtn->setObjectName("play_ToolBtn");
    this->m_like_ToolBtn->setObjectName("like_ToolBtn");
    this->m_more_ToolBtn->setObjectName("more_ToolBtn");

    this->m_coverLab->setFixedSize(60,60);
    this->m_coverLab->setPixmap(coverPix);
    this->m_coverLab->setScaledContents(true);

    this->m_play_ToolBtn->setIcon(QIcon("://image/tabIcon/play3-gray.svg"));
    this->m_like_ToolBtn->setIcon(QIcon("://image/playbar/collect.svg"));
    this->m_more_ToolBtn->setIcon(QIcon("://image/playbar/more.svg"));

    initUi();

    this->m_play_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
}

void ItemListWidget::paintEvent(QPaintEvent *ev)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ItemListWidget::enterEvent(QEnterEvent *ev)
{
    this->m_play_ToolBtn->show();
    this->m_like_ToolBtn->show();
    this->m_more_ToolBtn->show();
}

void ItemListWidget::leaveEvent(QEvent *ev)
{
    this->m_play_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
}

void ItemListWidget::initUi()
{
    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_authorLab);

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_coverLab);
    hlayout->addLayout(vlayout);
    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_play_ToolBtn);
    hlayout->addWidget(this->m_like_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);


}
