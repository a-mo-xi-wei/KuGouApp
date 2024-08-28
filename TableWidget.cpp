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

TableWidget::TableWidget(const QString &title, QWidget *parent)
    :QWidget(parent)
{
    this->m_titleLab = new QLabel(title,this);
    this->m_play_ToolBtn = new QToolButton(this);
    this->m_adjust_ToolBtn = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_ToolBtn = new QToolButton(this);
    this->m_tabWidget = new QWidget;

    this->m_titleLab->setObjectName("titleLab");
    this->m_play_ToolBtn->setObjectName("play_ToolBtn");
    this->m_adjust_ToolBtn->setObjectName("adjust_ToolBtn");
    this->m_refresh_ToolBtn->setObjectName("refresh_ToolBtn");
    this->m_more_ToolBtn->setObjectName("more_ToolBtn");
    this->m_tabWidget->setObjectName("tabWidget");

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
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(this->m_titleLab);
    hlayout->addWidget(this->m_play_ToolBtn);
    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_adjust_ToolBtn);
    hlayout->addWidget(this->m_refresh_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(this->m_tabWidget);
}



ItemListWidget::ItemListWidget(const QPixmap &coverPix, const QString &name, const QString &author, QWidget *parent)
    :QWidget(parent)
{

}
