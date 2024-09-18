#include "TableWidget.h"
#include"SMaskWidget.h"
#include<QLabel>
#include<QToolButton>
#include<QPaintEvent>
#include<QEnterEvent>
#include<QLayout>
#include<QSpacerItem>
#include<QStyleOption>
#include<QPainter>
#include<QGridLayout>
#include<QPainterPath>

TableWidget::TableWidget(const QString &title, QWidget *parent)
    :QWidget(parent)
{
    this->m_titleLab        = new QLabel(title,this);
    this->m_play_ToolBtn    = new QToolButton(this);
    this->m_adjust_ToolBtn  = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_Lab        = new QLabel("更多 >",this);
    this->m_tabWidget       = new QWidget;

    initUi();

    this->m_adjust_ToolBtn->hide();
    connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {this->m_tabWidget->setHidden(!this->m_tabWidget->isHidden()); });
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
    this->m_adjust_ToolBtn->setToolTip("折叠");
    this->m_refresh_ToolBtn->setToolTip("刷新");

    this->m_titleLab->setObjectName("titleLab");
    this->m_play_ToolBtn->setObjectName("play_ToolBtn");
    this->m_adjust_ToolBtn->setObjectName("adjust_ToolBtn");
    this->m_refresh_ToolBtn->setObjectName("refresh_ToolBtn");
    this->m_more_Lab->setObjectName("more_ToolBtn");
    this->m_tabWidget->setObjectName("tabWidget");

    //this->m_play_ToolBtn->setIcon(QIcon(":///Res/tabIcon/play2-gray.svg"));
    this->m_adjust_ToolBtn->setIcon(QIcon(":///Res/tabIcon/adjust-column-gray.svg"));
    this->m_refresh_ToolBtn->setIcon(QIcon(":///Res/tabIcon/refresh-gray.svg"));
    this->m_more_Lab->setStyleSheet("color:gray;font-size:12px;padding-bottom: 3px;");

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->setStyleSheet(R"(QLabel#titleLab{font-size:20px;}
                           QToolButton{background-color:rgba(255,255,255,0);}
                           QToolButton#play_ToolBtn{border-image: url(':///Res/tabIcon/play2-gray.svg');}
                           QToolButton#play_ToolBtn:hover{border-image: url(':///Res/tabIcon/play2-blue.svg');}
                         

)");
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(this->m_titleLab);
    hlayout->addWidget(this->m_play_ToolBtn);
    hlayout->addSpacerItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
    hlayout->addWidget(this->m_adjust_ToolBtn);
    hlayout->addWidget(this->m_refresh_ToolBtn);
    hlayout->addWidget(this->m_more_Lab);

    QGridLayout* glayout = new QGridLayout(this->m_tabWidget);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto item = new ItemListWidget(QPixmap(":///Res/tabIcon/music-cover.jpg"),"歌曲名字","作者",this->m_tabWidget);
            //item->setStyleSheet("border-radius:8px;");
            glayout->addWidget(item,i,j);
        }
    }

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(this->m_tabWidget);
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
    ,m_mask(new SMaskWidget(this))
{
    this->setFixedHeight(60);
    this->m_coverLab         = new QLabel(this);
    this->m_nameLab          = new QLabel(name,this);
    this->m_authorLab        = new QLabel(author,this);
    this->m_play_add_ToolBtn = new QToolButton(this);
    this->m_like_ToolBtn     = new QToolButton(this);
    this->m_more_ToolBtn     = new QToolButton(this);
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
}

void ItemListWidget::paintEvent(QPaintEvent *ev)
{
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);

    p.setRenderHint(QPainter::Antialiasing);
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
    this->setStyleSheet(R"(QWidget{border-radius:10px;})");
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

    this->m_like_ToolBtn->setIcon(QIcon(""));
    this->m_more_ToolBtn->setIcon(QIcon(""));
    
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
