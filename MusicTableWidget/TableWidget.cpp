#include "TableWidget.h"

#include<QLabel>
#include<QToolButton>
#include<QPaintEvent>
#include<QEnterEvent>
#include<QBoxLayout>
#include<QSpacerItem>
#include<QStyleOption>
#include<QPainter>
#include<QGridLayout>
#include<QPainterPath>

TableWidget::TableWidget(const QString &title, KIND kind ,QWidget *parent)
    :QWidget(parent)
    ,m_titleLab(new QLabel(title,this))
    ,m_kindList(kind)
    ,m_tabHLayout(std::make_unique<QHBoxLayout>())
{
    this->m_play_ToolBtn    = new QToolButton(this);
    this->m_adjust_ToolBtn  = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_Lab        = new QLabel("更多 >",this);

    initUi();

    this->m_adjust_ToolBtn->hide();
    connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {emit hide();});
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
    this->m_play_ToolBtn->setToolTip("开始播放");
    this->m_adjust_ToolBtn->setToolTip("隐藏栏目");
    this->m_refresh_ToolBtn->setToolTip("刷新");

    this->m_play_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_adjust_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_refresh_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_titleLab->setObjectName("titleLab");
    this->m_play_ToolBtn->setObjectName("play_ToolBtn");
    this->m_adjust_ToolBtn->setObjectName("adjust_ToolBtn");
    this->m_refresh_ToolBtn->setObjectName("refresh_ToolBtn");
    this->m_more_Lab->setObjectName("moreLab");

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->setStyleSheet(R"(QLabel#titleLab{font-size:20px;}
                           QLabel#moreLab{color:gray;font-size:12px;padding-bottom: 3px;}
                           QLabel#moreLab:hover{color:#26a1ff;}
                           QToolButton{background-color:rgba(255,255,255,0);}
                           QToolButton#adjust_ToolBtn{border-image: url(':///Res/tabIcon/adjust-column-gray.svg');}
                           QToolButton#adjust_ToolBtn:hover{border-image: url(':///Res/tabIcon/adjust-column-blue.svg');}
                           QToolButton#refresh_ToolBtn{border-image: url(':///Res/tabIcon/refresh-gray.svg');}
                           QToolButton#refresh_ToolBtn:hover{border-image: url(':///Res/tabIcon/refresh-blue.svg');}
                           QToolButton#play_ToolBtn{border-image: url(':///Res/tabIcon/play2-gray.svg');}
                           QToolButton#play_ToolBtn:hover{border-image: url(':///Res/tabIcon/play2-blue.svg');}
                        )");

    m_tabHLayout->addWidget(this->m_titleLab);
    m_tabHLayout->addWidget(this->m_play_ToolBtn);
    m_tabHLayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    m_tabHLayout->addWidget(this->m_adjust_ToolBtn);
    m_tabHLayout->addWidget(this->m_refresh_ToolBtn);
    m_tabHLayout->addWidget(this->m_more_Lab);

    if(this->m_kindList == KIND::ItemList){
        initItemListWidget();
    }
    else if(this->m_kindList == KIND::BlockList){
        this->m_play_ToolBtn->hide();
        this->m_more_Lab->setText("歌单广场 >");
        initBlockListWidget();
    }
}
void TableWidget::initBlockListWidget()
{
    QGridLayout* glayout = new QGridLayout;
    glayout->setSpacing(15);
    for (int i = 1; i <= 2; ++i) {
        for (int j = 1; j <= 5; ++j) {
            QString pixPath = QString(":/Res/tabIcon/music-block-cover%1.jpg").arg(i*j);
            auto block = new ItemBlockWidget(pixPath,this);
            glayout->addWidget(block,i-1,j-1);
        }
    }
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addLayout(glayout);
}

void TableWidget::initItemListWidget()
{
    QGridLayout* glayout = new QGridLayout;
    glayout->setSpacing(15);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QString pixPath = QString(":///Res/tabIcon/music-cover.jpg");
            auto item = new ItemListWidget(QPixmap(pixPath),"歌曲名字","作者",this);
            glayout->addWidget(item,i,j);
        }
    }

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addLayout(glayout);
}

QPixmap roundedPixmap(QPixmap& src, QSize size, int radius) {
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

ItemListWidget::ItemListWidget(QPixmap coverPix, const QString &name, const QString &author, QWidget *parent)
    :QWidget(parent)
    ,m_mask(std::make_unique<SMaskWidget>(this))
    ,m_coverLab(new QLabel(this))
    ,m_nameLab(new QLabel(name,this))
    ,m_authorLab(new QLabel(author,this))
    ,m_play_add_ToolBtn(new QToolButton(this))
    ,m_like_ToolBtn(new QToolButton(this))
    ,m_more_ToolBtn(new QToolButton(this))
{
    this->setFixedHeight(65);
    this->m_coverLab->setFixedSize(this->height(),this->height());
    this->m_coverLab->setPixmap(roundedPixmap(coverPix,this->m_coverLab->size(),8));
    this->m_coverLab->setScaledContents(true);
    this->m_coverLab->setStyleSheet(R"(margin:0px;padding:0px;background-color: rgba(255, 0, 0, 0);)");
    initUi();
    this->m_mask->move(this->m_coverLab->pos());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    this->m_mask->hide();
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
    connect(qobject_cast<TableWidget*>(parent),&TableWidget::hide,this,&ItemListWidget::onHide);
}

void ItemListWidget::paintEvent(QPaintEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);

    p.setRenderHint(QPainter::Antialiasing,true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
    }else{
        this->m_mask->hide();
    }

}

void ItemListWidget::enterEvent(QEnterEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    this->m_isHoverCoverLab = true;
    this->m_play_add_ToolBtn->show();
    this->m_like_ToolBtn->show();
    this->m_more_ToolBtn->show();
    this->setStyleSheet(R"(QWidget{border-radius:8px;background-color:#DDDDDD;}
                           QLabel{background-color:#DDDDDD;}
                           QLabel:hover{color:#2291e6;})");
    update();
}

void ItemListWidget::leaveEvent(QEvent *ev)
{
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
    this->setStyleSheet(R"(QWidget{border-radius:8px;})");
    update();
}

void ItemListWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / this->m_aspectRatio);
    this->m_coverLab->setFixedSize(this->height(),this->height());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    update();

}

void ItemListWidget::initUi()
{
    this->m_coverLab->setCursor(Qt::PointingHandCursor);
    this->m_nameLab->setCursor(Qt::PointingHandCursor);
    this->m_authorLab->setCursor(Qt::PointingHandCursor);
    this->m_play_add_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_like_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_more_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_coverLab->setObjectName("coverLab");
    this->m_nameLab->setObjectName("nameLab");
    this->m_authorLab->setObjectName("authorLab");
    this->m_play_add_ToolBtn->setObjectName("play_add_ToolBtn");
    this->m_like_ToolBtn->setObjectName("like_ToolBtn");
    this->m_more_ToolBtn->setObjectName("more_ToolBtn");

    
    this->m_play_add_ToolBtn->setStyleSheet(R"(QToolButton#play_add_ToolBtn{border-image: url(':///Res/tabIcon/play-add-gray.svg');}
                                               QToolButton#play_add_ToolBtn:hover{border-image: url(':///Res/tabIcon/play-add-blue.svg');})");
    this->m_like_ToolBtn->setStyleSheet(R"(QToolButton#like_ToolBtn{border-image: url(':///Res/playbar/collect.svg')}
                                           QToolButton#like_ToolBtn:hover{border-image: url(':///Res/tabIcon/collect-blue.svg');})");
    this->m_more_ToolBtn->setStyleSheet(R"(QToolButton#more_ToolBtn{border-image: url(':///Res/playbar/more.svg');}
                                           QToolButton#more_ToolBtn:hover{border-image: url(':///Res/playbar/more-blue.svg');})");

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_authorLab);

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0,0,10,0);
    hlayout->addWidget(this->m_coverLab);

    hlayout->addLayout(vlayout);

    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_play_add_ToolBtn);
    hlayout->addWidget(this->m_like_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);


}

void ItemListWidget::onHide()
{
    this->setHidden(!this->isHidden());
    update();
}

#define FontHeight 25
ItemBlockWidget::ItemBlockWidget(const QString& path, QWidget *parent)
    :QWidget(parent)
    ,m_bacWidget(new QWidget(this))
    ,m_mask(std::make_unique<SMaskWidget>(this))
    ,m_describeLab(new QLabel(this))
    ,m_tipLab(new QLabel(this))
    ,m_popularBtn(new QToolButton(this))
{
    QString style = QString("border-radius:8px;border-image:url(%1);").arg(path);
    this->m_bacWidget->setStyleSheet(style);
    this->setFixedHeight(150 + FontHeight);
    initUi();
    this->m_mask->setParent(this->m_bacWidget);
    this->m_mask->move(this->m_bacWidget->pos());
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_mask->hide();
    connect(qobject_cast<TableWidget*>(parent),&TableWidget::hide,this,&ItemBlockWidget::onHide);
}

void ItemBlockWidget::paintEvent(QPaintEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);

    p.setRenderHint(QPainter::Antialiasing,true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);");
    }else{
        this->m_mask->hide();
    }

}

void ItemBlockWidget::enterEvent(QEnterEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    this->m_isHoverCoverLab = true;
    update();
}

void ItemBlockWidget::leaveEvent(QEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    update();
}

void ItemBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //qDebug()<<"改变大小 : "<<event->size();
    this->setFixedHeight(event->size().width()+FontHeight);
    this->m_bacWidget->setFixedSize(event->size().width()/1.1,event->size().width()/1.1);
    //this->m_bacWidget->setFixedSize(event->size().width(),event->size().width());
    //qDebug()<<"this->m_bacWidget->width() : "<<this->m_bacWidget->width()<<"\nthis->width() : "<<this->width();
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_popularBtn->move(this->m_bacWidget->width()-this->m_popularBtn->width()-5,
                             this->m_bacWidget->height()-this->m_popularBtn->height()-5);
    update();
}

void ItemBlockWidget::initUi()
{
    this->setCursor(Qt::PointingHandCursor);
    this->m_describeLab->setObjectName("describeLab");
    this->m_tipLab->setObjectName("tipLab");
    this->m_popularBtn->setObjectName("popularLab");

    this->m_tipLab->setFixedSize(50,20);
    this->m_tipLab->setText("网络");
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet("border-radius:10px;background-color:black;color:white;");
    this->m_tipLab->move(10,10);

    this->m_popularBtn->setFixedSize(70,20);
    this->m_popularBtn->setIcon(QIcon("://Res/tabIcon/popular-white.svg"));
    this->m_popularBtn->setText(" 123.1万");
    this->m_popularBtn->setContentsMargins(5,0,5,0);
    this->m_popularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_popularBtn->setStyleSheet("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);");
    //this->m_popularBtn->move(this->width()-this->m_popularBtn->height()-10,this->m_bacWidget->height()-this->m_popularBtn->height()-10);
    //qDebug()<<"this->m_bacWidget->width() : "<<this->m_bacWidget->width()<<"\nthis->width() : "<<this->width();

    this->m_describeLab->setText("游戏主播常用音乐");
    this->m_describeLab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    this->m_describeLab->setFixedSize(this->width(),FontHeight);


    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget(this->m_bacWidget);
    vlayout->addWidget(this->m_describeLab);


}

void ItemBlockWidget::onHide()
{
    this->setHidden(!this->isHidden());
    update();
}
