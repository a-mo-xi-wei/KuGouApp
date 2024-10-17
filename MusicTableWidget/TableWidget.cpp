#include "TableWidget.h"

#include<QLabel>
#include<QToolButton>
#include<QTimer>
#include<QEnterEvent>
#include<QBoxLayout>
#include<QSpacerItem>
#include<QStyleOption>
#include<QPainter>
#include<QPainterPath>
#include<QRandomGenerator>
#include<vector>



TableWidget::TableWidget(const QString &title, KIND kind, QWidget *parent)
    : QWidget(parent)
      , m_titleLab(new QLabel(title, this))
      , m_kindList(kind)
      , m_tabHLayout(std::make_unique<QHBoxLayout>())
      , m_gridLayout(std::make_unique<QGridLayout>())
      , m_gridContainer(std::make_unique<QWidget>(this))
      , m_refreshTimer(new QTimer(this))
{
    this->m_play_ToolBtn = new QToolButton(this);
    this->m_adjust_ToolBtn = new QToolButton(this);
    this->m_refresh_ToolBtn = new QToolButton(this);
    this->m_more_Lab = new QLabel(QStringLiteral("更多 >"), this);

    initBlockCover();
    initListCover();
    initUi();

    this->m_adjust_ToolBtn->hide();
    if(kind == KIND::ItemList){
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {emit hideTitle();});
    }
    else if(kind == KIND::BlockList) {
        connect(this->m_adjust_ToolBtn, &QToolButton::clicked, this, [this] {
            this->m_gridContainer->setHidden(!this->m_gridContainer->isHidden());
        });
    }

    connect(this, &TableWidget::gridChange, this, [this](int len) { onGridChange(len); });

    connect(this->m_refresh_ToolBtn,&QToolButton::clicked,this,&TableWidget::onRefreshBtnClicked);
    connect(this->m_refreshTimer,&QTimer::timeout,this,&TableWidget::onRefreshTimeout);
}

void TableWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TableWidget::enterEvent(QEnterEvent *ev) {
    this->m_adjust_ToolBtn->show();
    this->line1->show();
}

void TableWidget::leaveEvent(QEvent *ev) {
    this->m_adjust_ToolBtn->hide();
    this->line1->hide();
}

void TableWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"this->width : "<<this->width();
    emit gridChange(this->width());
}

void TableWidget::initUi() {

    this->m_play_ToolBtn->setToolTip(QStringLiteral("开始播放"));
    this->m_adjust_ToolBtn->setToolTip(QStringLiteral("隐藏栏目"));
    this->m_refresh_ToolBtn->setToolTip(QStringLiteral("刷新"));

    this->m_play_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_adjust_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_refresh_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_titleLab->setObjectName(QStringLiteral("titleLab"));
    this->m_play_ToolBtn->setObjectName(QStringLiteral("play_ToolBtn"));
    this->m_adjust_ToolBtn->setObjectName(QStringLiteral("adjust_ToolBtn"));
    this->m_refresh_ToolBtn->setObjectName(QStringLiteral("refresh_ToolBtn"));
    this->m_more_Lab->setObjectName(QStringLiteral("moreLab"));

    this->m_play_ToolBtn->setIconSize(QSize(20, 20));

    this->m_refreshTimer->setSingleShot(true);

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

    m_tabHLayout->setSpacing(0);
    m_tabHLayout->addWidget(this->m_titleLab);
    m_tabHLayout->addWidget(this->m_play_ToolBtn);
    m_tabHLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
    m_tabHLayout->addWidget(this->m_adjust_ToolBtn);
    this->line1 = new QLabel(this);
    line1->hide();
    line1->setScaledContents(true);
    line1->setFixedSize(20, 15);
    line1->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-gray.svg")));
    m_tabHLayout->addWidget(line1);
    m_tabHLayout->addWidget(this->m_refresh_ToolBtn);
    const auto line2 = new QLabel(this);
    line2->setScaledContents(true);
    line2->setFixedSize(20, 15);
    line2->setPixmap(QPixmap(QStringLiteral(":/Res/tabIcon/line-gray.svg")));
    m_tabHLayout->addWidget(line2);
    m_tabHLayout->addWidget(this->m_more_Lab);

    if (this->m_kindList == KIND::ItemList) {
        initItemListWidget();
    }
    else if (this->m_kindList == KIND::BlockList) {
        this->m_play_ToolBtn->hide();
        this->m_more_Lab->setText(QStringLiteral("歌单广场 >"));
        initItemBlockWidget();
    }
}

void TableWidget::initBlockCover() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            this->m_blockCoverPaths.emplace_back(QString(":/Res/tabIcon/music-block-cover%1.jpg").arg(i * 7 + j + 1));
        }
    }
}

void TableWidget::shuffleBlockCover() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_blockCoverPaths.begin(), this->m_blockCoverPaths.end(), std::default_random_engine(seed));
}

void TableWidget::initListCover() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            this->m_listCoverPaths.emplace_back(QString(":/Res/tabIcon/music-list-cover%1.jpg").arg(i * 3 + j + 1));
        }
    }
    shuffleListCover();

}

void TableWidget::shuffleListCover() {
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_listCoverPaths.begin(), this->m_listCoverPaths.end(), std::default_random_engine(seed));
}

void TableWidget::initItemBlockWidget() {
    this->m_gridContainer->setLayout(m_gridLayout.get());
    this->m_gridLayout->setVerticalSpacing(10);
    this->m_gridLayout->setHorizontalSpacing(5);
    this->m_gridLayout->setContentsMargins(0,5,0,5);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 7; ++j) {
            QString pixPath = this->m_blockCoverPaths[i * 7 + j];
            auto block = new ItemBlockWidget(pixPath, this);
            block->hide();
            this->m_gridLayout->addWidget(block, i, j);
        }
    }
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < this->m_showCol; ++j) {
            this->m_gridLayout->itemAtPosition(i, j)->widget()->show();
        }
    }
    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0,5,0,5);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addWidget(this->m_gridContainer.get());
}

void TableWidget::initItemListWidget() {
    this->m_gridLayout->setSpacing(10);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto pixPath = this->m_listCoverPaths[i*3+j];
            auto item = new ItemListWidget(pixPath, QStringLiteral("歌曲名字"), QStringLiteral("作者"), this);
            this->m_gridLayout->addWidget(item, i, j);
        }
    }

    auto vlayout = new QVBoxLayout(this);
    vlayout->addLayout(m_tabHLayout.get());
    vlayout->addLayout(this->m_gridLayout.get());
}

void TableWidget::onGridChange(int len) {
    this->m_hide_col_6 = true;
    this->m_hide_col_7 = true;
    auto item = this->m_gridLayout->itemAtPosition(0, 5);
    if (item) {
        this->m_hide_col_6 = item->widget()->isHidden();
        //qDebug()<<"hide_col_6 : "<<hide_col_6;
    }
    item = this->m_gridLayout->itemAtPosition(0, 6);
    if (item) {
        this->m_hide_col_7 = item->widget()->isHidden();
        //qDebug()<<"hide_col_7 : "<<hide_col_7;
    }

    if (this->m_hide_col_6) {
        //显示前5列
        //if(this->m_gridLayout->columnCount() == 7){
        this->m_showCol = 5;
        if (len < 1000)return;
        if (len < 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if(item)item->widget()->show();
                //qDebug()<<"show一个";
            }
            this->m_showCol = 6;
            //qDebug()<<"//////////////////////////";
        }
        //由于直接全屏需要一定时间，因此不存在从5列直接变成7列的情况
        //但是，上面说的是正常缩放的情况，但如果存在突然发送的信号的情况。。。
        else {
            this->m_showCol = 7;
        }
    }
    else if (!this->m_hide_col_6 && this->m_hide_col_7) {
        //显示前6列
        this->m_showCol = 6;
        if (len < 1000) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 5);
                if(item)item->widget()->hide();
            }
            this->m_showCol = 5;
        } else if (len >= 1200) {
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if(item)item->widget()->show();
            }
            this->m_showCol = 7;
        }
    }
    else if (!this->m_hide_col_7) {
        this->m_showCol = 7;
        //显示前7列
        if(len < 1000) {
            this->m_showCol = 5;
        }
        else if(len < 1200){
            for (int i = 0; i < 2; ++i) {
                auto item = this->m_gridLayout->itemAtPosition(i, 6);
                if(item)item->widget()->hide();
            }
            this->m_showCol = 6;
            //qDebug()<<"********************";
        }
    }
}

void TableWidget::onRefreshBtnClicked() {
    // 启动定时器，延迟处理
    if (!this->m_refreshTimer->isActive()) {
        this->m_refreshTimer->start(200);  // 500ms 延迟，避免过于频繁地触发
    }
}

void TableWidget::onRefreshTimeout() {
    if(this->m_kindList == KIND::BlockList) {
        shuffleBlockCover();
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 7; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i,j);
                if(item) {
                    auto widget = item->widget();
                    if(widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                QString pixPath = this->m_blockCoverPaths[i * 7 + j];
                auto block = new ItemBlockWidget(pixPath, this);
                block->hide();
                this->m_gridLayout->addWidget(block, i, j);
            }
        }
        emit gridChange(this->width());
        //qDebug()<<"当前显示 ： "<<this->m_showCol;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < this->m_showCol; ++j) {
                this->m_gridLayout->itemAtPosition(i, j)->widget()->show();
            }
        }

    }
    else {
        shuffleListCover();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                auto item = this->m_gridLayout->itemAtPosition(i,j);
                if(item) {
                    auto widget = item->widget();
                    if(widget) {
                        this->m_gridLayout->removeWidget(widget);
                        widget->deleteLater();
                    }
                }
                QString pixPath = this->m_listCoverPaths[i * 3 + j];
                auto list = new ItemListWidget(pixPath, QStringLiteral("歌曲名字"), QStringLiteral("作者"), this);
                this->m_gridLayout->addWidget(list, i, j);
            }
        }
    }
}

QPixmap roundedPixmap(QPixmap &src, QSize size, int radius) {
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
    : QWidget(parent)
      , m_mask(std::make_unique<SMaskWidget>(this))
      , m_coverLab(new QLabel(this))
      , m_nameLab(new QLabel(name, this))
      , m_authorLab(new QLabel(author, this))
      , m_play_add_ToolBtn(new QToolButton(this))
      , m_like_ToolBtn(new QToolButton(this))
      , m_more_ToolBtn(new QToolButton(this)) {
    this->setFixedHeight(65);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_coverLab->setPixmap(roundedPixmap(coverPix, this->m_coverLab->size(), 8));
    this->m_coverLab->setScaledContents(true);
    this->m_coverLab->setStyleSheet(R"(margin:0px;padding:0px;background-color: rgba(255, 0, 0, 0);)");
    initUi();
    this->m_mask->move(this->m_coverLab->pos());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    this->m_mask->hide();
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
    connect(qobject_cast<TableWidget *>(parent), &TableWidget::hideTitle, this, &ItemListWidget::onHide);
}

void ItemListWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
    } else {
        this->m_mask->hide();
    }
}

void ItemListWidget::enterEvent(QEnterEvent *ev) {
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

void ItemListWidget::leaveEvent(QEvent *ev) {
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    this->m_play_add_ToolBtn->hide();
    this->m_like_ToolBtn->hide();
    this->m_more_ToolBtn->hide();
    this->setStyleSheet(R"(QWidget{border-radius:8px;})");
    update();
}

void ItemListWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / this->m_aspectRatio);
    this->m_coverLab->setFixedSize(this->height(), this->height());
    this->m_mask->setFixedSize(this->m_coverLab->size());
    update();
}

void ItemListWidget::initUi() {
    this->m_coverLab->setCursor(Qt::PointingHandCursor);
    this->m_nameLab->setCursor(Qt::PointingHandCursor);
    this->m_authorLab->setCursor(Qt::PointingHandCursor);
    this->m_play_add_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_like_ToolBtn->setCursor(Qt::PointingHandCursor);
    this->m_more_ToolBtn->setCursor(Qt::PointingHandCursor);

    this->m_coverLab->setObjectName(QStringLiteral("coverLab"));
    this->m_nameLab->setObjectName(QStringLiteral("nameLab"));
    this->m_authorLab->setObjectName(QStringLiteral("authorLab"));
    this->m_play_add_ToolBtn->setObjectName(QStringLiteral("play_add_ToolBtn"));
    this->m_like_ToolBtn->setObjectName(QStringLiteral("like_ToolBtn"));
    this->m_more_ToolBtn->setObjectName(QStringLiteral("more_ToolBtn"));


    this->m_play_add_ToolBtn->setStyleSheet(
        R"(QToolButton#play_add_ToolBtn{border-image: url(':///Res/tabIcon/play-add-gray.svg');}
                                               QToolButton#play_add_ToolBtn:hover{border-image: url(':///Res/tabIcon/play-add-blue.svg');})");
    this->m_like_ToolBtn->setStyleSheet(R"(QToolButton#like_ToolBtn{border-image: url(':///Res/playbar/collect.svg')}
                                           QToolButton#like_ToolBtn:hover{border-image: url(':///Res/tabIcon/collect-blue.svg');})");
    this->m_more_ToolBtn->setStyleSheet(R"(QToolButton#more_ToolBtn{border-image: url(':///Res/playbar/more.svg');}
                                           QToolButton#more_ToolBtn:hover{border-image: url(':///Res/playbar/more-blue.svg');})");

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(this->m_nameLab);
    vlayout->addWidget(this->m_authorLab);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->setContentsMargins(0, 0, 10, 0);
    hlayout->addWidget(this->m_coverLab);

    hlayout->addLayout(vlayout);

    hlayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
    hlayout->addWidget(this->m_play_add_ToolBtn);
    hlayout->addWidget(this->m_like_ToolBtn);
    hlayout->addWidget(this->m_more_ToolBtn);
}

void ItemListWidget::onHide() {
    this->setHidden(!this->isHidden());
    update();
}

#define FontHeight 25
std::vector<QString> tipArr =
    {QStringLiteral("日语"),QStringLiteral("经典"),
    QStringLiteral("国语"), QStringLiteral("流行"),
    QStringLiteral("兴奋"), QStringLiteral("英语"),
    QStringLiteral("古风"), QStringLiteral("韩语"),
    QStringLiteral("寂寞"), QStringLiteral("运动"),
    QStringLiteral("说唱"), QStringLiteral("校园") };

ItemBlockWidget::ItemBlockWidget(const QString &path, QWidget *parent)
    : QWidget(parent)
      , m_bacWidget(new QWidget(this))
      , m_mask(std::make_unique<SMaskWidget>(this))
      , m_describeLab(new QLabel(this))
      , m_tipLab(new QLabel(this))
      , m_popularBtn(new QToolButton(this)) {
    QString style = QString("border-radius:8px;border-image:url(%1);").arg(path);
    this->m_bacWidget->setStyleSheet(style);
    this->setFixedHeight(150 + FontHeight);
    initUi();
    this->m_mask->setParent(this->m_bacWidget);
    this->m_mask->move(this->m_bacWidget->pos());
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_mask->hide();
}

void ItemBlockWidget::setTipLabText(const QString &text) {
    this->m_tipLab->setText(text);
}

void ItemBlockWidget::setPopularBtnText(const QString &text) {
    this->m_popularBtn->setText(" " + text + "万");
}

void ItemBlockWidget::paintEvent(QPaintEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    // 如果鼠标悬停，绘制半透明蒙层
    if (this->m_isHoverCoverLab) {
        this->m_mask->show();
        this->m_mask->raise();
        this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(60,60,60, 127);"));
    } else {
        this->m_mask->hide();
    }
}

void ItemBlockWidget::enterEvent(QEnterEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::enterEvent(ev);
    this->m_isHoverCoverLab = true;
    update();
}

void ItemBlockWidget::leaveEvent(QEvent *ev) {
    // 先调用父类的 paintEvent 以执行默认绘制行为
    QWidget::leaveEvent(ev);
    this->m_isHoverCoverLab = false;
    update();
}

void ItemBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"改变大小 : "<<event->size();

    this->setFixedHeight(event->size().width() + FontHeight);
    //只要留了一定的空间，就能缩小
    this->m_bacWidget->setFixedSize(event->size().width() / 1.05, event->size().width() / 1.05);
    //this->m_bacWidget->setFixedSize(event->size().width(),event->size().width());
    //qDebug()<<"this->m_bacWidget->size() : "<<this->m_bacWidget->size();
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_describeLab->setFixedSize(this->m_bacWidget->width(),FontHeight);
    this->m_popularBtn->move(this->m_bacWidget->width() - this->m_popularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_popularBtn->height() - 5);
    update();
}

void ItemBlockWidget::initUi() {
    this->setCursor(Qt::PointingHandCursor);
    this->m_describeLab->setObjectName(QStringLiteral("describeLab"));
    this->m_tipLab->setObjectName(QStringLiteral("tipLab"));
    this->m_popularBtn->setObjectName(QStringLiteral("popularLab"));

    this->m_tipLab->setFixedSize(50, 20);
    this->setTipLabText(tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(tipArr.size()))]);
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;"));
    this->m_tipLab->move(10, 10);

    this->m_popularBtn->setFixedSize(70, 20);
    this->m_popularBtn->setIcon(QIcon(QStringLiteral("://Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_popularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_popularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_popularBtn->setStyleSheet(QStringLiteral("color:white;border-radius:10px;background-color: rgba(128, 128, 128, 127);"));
    //this->m_popularBtn->move(this->width()-this->m_popularBtn->height()-10,this->m_bacWidget->height()-this->m_popularBtn->height()-10);
    //qDebug()<<"this->m_bacWidget->width() : "<<this->m_bacWidget->width()<<"\nthis->width() : "<<this->width();

    this->m_describeLab->setText(QStringLiteral("游戏主播常用音乐"));
    this->m_describeLab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    auto vlayout = new QVBoxLayout(this);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->addWidget(this->m_bacWidget);
    vlayout->addWidget(this->m_describeLab);
}

