#include "KuGouApp.h"
#include "ui_KuGouApp.h"

#include<QMediaMetaData>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QPainterPath>
#include<QPainter>
#include<QPixmap>
#include<QDir>
#include<QFile>
#include<QPoint>
#include<QMouseEvent>
#include<QButtonGroup>
#include<QSizeGrip>
#include <QPropertyAnimation>
static QRegularExpression re("^[A-Za-z0-9\\p{Han}\\\\/\\-_\\*]+$");

QPixmap roundedPixmap(const QPixmap &src, QSize size, int radius) {
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

KuGouApp::KuGouApp(MainWindow *parent)
    : MainWindow(parent)
      , ui(new Ui::KuGouApp)
      , m_player(std::make_unique<QMediaPlayer>(this))
      , m_audioOutput(std::make_unique<QAudioOutput>(this))
      , m_menuBtnGroup(std::make_unique<QButtonGroup>(this))
      , m_sizeGrip(std::make_unique<QSizeGrip>(this))
      , m_animation(std::make_unique<QPropertyAnimation>(this,"geometry"))
      , m_recommendForYou(std::make_unique<RecommendForYou>(this))
      , m_localDownload(std::make_unique<LocalDownload>(this)) {
    ui->setupUi(this);
    QFile file("://Res/styles/original.css");
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    this->m_player->setAudioOutput(this->m_audioOutput.get());
    this->m_audioOutput->setVolume(0.15);
    connect(ui->volume_toolButton,&VolumeToolBtn::volumeChange,this,[this](const int value) {
        const float volume = static_cast<float>(value) / 100; // 将值转换为0.0到1.0之间
        this->m_audioOutput->setVolume(volume); // 设置音量
    });

    connect(this->m_player.get(), &QMediaPlayer::positionChanged,this, [this](int position) {
        if(ui->progressSlider->isSliderDown())return;
        //qDebug()<<"position "<<position;
        ui->progressSlider->setValue(position);
        ui->position_label->setText(QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss"));
    });
    connect(this->m_player.get(), &QMediaPlayer::durationChanged, this, &KuGouApp::updateSliderRange);
    connect(this->m_player.get(), &QMediaPlayer::metaDataChanged, this, [this] {
        //qDebug()<<"metaDataChanged";
        ui->cover_label->setPixmap(roundedPixmap(this->m_songInfor.cover,ui->cover_label->size(),8));
        ui->song_name_label->setText(this->m_songInfor.songName);
        ui->singer_label->setText(this->m_songInfor.signer);
    });
    connect(this->m_player.get(), &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
       if(state == QMediaPlayer::PlayingState)this->m_isPlaying = true;
       else this->m_isPlaying = false;
        if (this->m_isPlaying) {
         ui->play_or_pause_toolButton->setIcon(QIcon("://Res/playbar/pause.svg"));
         } else {
             ui->play_or_pause_toolButton->setIcon(QIcon("://Res/playbar/play.svg"));
         }
    });

    connect(ui->progressSlider,&QSlider::sliderReleased, this,&KuGouApp::updateProcess);
    connect(ui->progressSlider,&QSlider::sliderMoved,this,&KuGouApp::updateProcess);

    connect(this->m_localDownload.get(),&LocalDownload::playMusic,this,&KuGouApp::onPlayMusic);

    ui->progressSlider->installEventFilter(this);
}

KuGouApp::~KuGouApp() {
    delete ui;
}

void KuGouApp::on_title_return_toolButton_clicked() {
    qDebug()<<"返回，估计要使用堆栈";
}

void KuGouApp::on_title_refresh_toolButton_clicked() {
    qDebug()<<"刷新界面";
    ui->center_widget->repaint();
}

void KuGouApp::on_title_music_pushButton_clicked() {
    ui->index_label1->show();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();
}

void KuGouApp::on_title_live_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->show();
    ui->index_label3->hide();
    ui->index_label4->hide();
}

void KuGouApp::on_title_listenBook_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->show();
    ui->index_label4->hide();
}

void KuGouApp::on_title_found_pushButton_clicked() {
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->show();
}

void KuGouApp::initUi() {
    this->setWindowIcon(QIcon("://Res/window/windowIcon.svg"));
    //去掉标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    move(QGuiApplication::primaryScreen()->geometry().width()/2-this->width()/2, 100);
    this->setMouseTracking(true);
    ui->title_widget->setMouseTracking(true);
    ui->center_widget->setMouseTracking(true);
    ui->play_widget->setMouseTracking(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_Hover,true);

    initTitleWidget();
    initCommendForYouWidget();
    initLocalDownload();
    initPlayWidget();
    initMenu();
    initLocalDownload();
    initCornerWidget();
}

void KuGouApp::initCommendForYouWidget() {
    ui->stackedWidget->addWidget(this->m_recommendForYou.get());
}

void KuGouApp::initLocalDownload() {
    ui->stackedWidget->addWidget(this->m_localDownload.get());
}


void KuGouApp::initTitleWidget() {
    ui->index_label1->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30, 15, Qt::KeepAspectRatio));
    ui->index_label2->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30, 15, Qt::KeepAspectRatio));
    ui->index_label3->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30, 15, Qt::KeepAspectRatio));
    ui->index_label4->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30, 15, Qt::KeepAspectRatio));
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();

    ui->title_line->setPixmap(QPixmap(":/Res/tabIcon/line-gray.svg"));
    ui->search_lineEdit->addAction(QIcon("://Res/titlebar/search-black.svg"), QLineEdit::LeadingPosition);

    //除非自定义QToolButton否则达不到 CSS 中 border-image 的效果
    //ui->listen_toolButton->setIcon(QIcon("://Res/titlebar/listen-music-black.svg"));

    QPixmap roundedPix = roundedPixmap(QPixmap("://Res/window/portrait.jpg"), ui->title_portrait_label->size(), 20);
    // 设置圆角半径
    ui->title_portrait_label->setPixmap(roundedPix);

    ui->title_gender_label->setPixmap(QPixmap("://Res/window/boy.svg"));

    //设置设置按钮的Frame圆角，填充颜色
    ui->min_toolButton->setRadius(6);
    ui->max_toolButton->setRadius(6);
    ui->close_toolButton->setRadius(6);

    ui->min_toolButton->setFillColor(QColor("#969696"));
    ui->max_toolButton->setFillColor(QColor("#969696"));
    ui->close_toolButton->setFillColor(QColor("#FF0066"));

    ui->min_toolButton->setMyIcon(QIcon("://Res/titlebar/minimize-black.svg"));
    ui->max_toolButton->setMyIcon(QIcon("://Res/titlebar/maximize-black.svg"));
    ui->close_toolButton->setMyIcon(QIcon("://Res/titlebar/close-black.svg"));

    connect(ui->title_widget, &TitleWidget::doubleClicked, this, [this] { ui->max_toolButton->click(); });
}

void KuGouApp::initPlayWidget() {
    ui->love_toolButton->setIcon(QIcon("://Res/playbar/collect.svg"));
    ui->download_toolButton->setIcon(QIcon("://Res/playbar/download.svg"));
    ui->comment_toolButton->setIcon(QIcon("://Res/playbar/comment.svg"));
    ui->share_toolButton->setIcon(QIcon("://Res/playbar/share.svg"));
    ui->more_toolButton->setIcon(QIcon("://Res/playbar/more.svg"));
    ui->pre_toolButton->setIcon(QIcon("://Res/playbar/previous-song.svg"));
    ui->play_or_pause_toolButton->setIcon(QIcon("://Res/playbar/play.svg"));
    ui->next_toolButton->setIcon(QIcon("://Res/playbar/next-song.svg"));
    ui->erji_toolButton->setIcon(QIcon("://Res/playbar/together.svg"));
    ui->ci_toolButton->setIcon(QIcon("://Res/playbar/song-words.svg"));
    ui->list_toolButton->setIcon(QIcon("://Res/playbar/play-list.svg"));

    connect(ui->play_widget, &PlayWidget::doubleClicked, this, [this] { ui->max_toolButton->click(); });
}

void KuGouApp::initMenu() {
    this->m_menuBtnGroup->setParent(ui->center_menu_widget);
    ui->recommend_toolButton->setIcon(QIcon("://Res/window/recommend.svg"));
    ui->yueku_toolButton->setIcon(QIcon("://Res/window/music-library.svg"));
    ui->gedan_toolButton->setIcon(QIcon("://Res/window/song-list.svg"));
    ui->pindao_toolButton->setIcon(QIcon("://Res/window/my-channel.svg"));
    ui->video_toolButton->setIcon(QIcon("://Res/window/video.svg"));
    ui->live_toolButton->setIcon(QIcon("://Res/window/live.svg"));
    ui->my_shoucang_toolButton->setIcon(QIcon("://Res/window/collect.svg"));
    ui->my_pindao_toolButton->setIcon(QIcon("://Res/window/my-channel.svg"));
    ui->local_download_toolButton->setIcon(QIcon("://Res/window/download.svg"));
    ui->music_yunpan_toolButton->setIcon(QIcon("://Res/window/cloud.svg"));
    ui->yigou_yunpan_toolButton->setIcon(QIcon("://Res/window/bought.svg"));
    ui->zuijin_bofang_toolButton->setIcon(QIcon("://Res/window/history.svg"));
    ui->moren_liebiao_toolButton->setIcon(QIcon("://Res/titlebar/menu-black.svg"));

    m_menuBtnGroup->addButton(ui->recommend_toolButton);
    m_menuBtnGroup->addButton(ui->yueku_toolButton);
    m_menuBtnGroup->addButton(ui->gedan_toolButton);
    m_menuBtnGroup->addButton(ui->pindao_toolButton);
    m_menuBtnGroup->addButton(ui->video_toolButton);
    m_menuBtnGroup->addButton(ui->live_toolButton);
    m_menuBtnGroup->addButton(ui->my_shoucang_toolButton);
    m_menuBtnGroup->addButton(ui->my_pindao_toolButton);
    m_menuBtnGroup->addButton(ui->local_download_toolButton);
    m_menuBtnGroup->addButton(ui->music_yunpan_toolButton);
    m_menuBtnGroup->addButton(ui->yigou_yunpan_toolButton);
    m_menuBtnGroup->addButton(ui->zuijin_bofang_toolButton);
    m_menuBtnGroup->addButton(ui->moren_liebiao_toolButton);
    m_menuBtnGroup->setExclusive(true);
}

void KuGouApp::initCornerWidget() {
    this->m_sizeGrip->setFixedSize(11, 11);
    this->m_sizeGrip->setObjectName("sizegrip");
}

void KuGouApp::mousePressEvent(QMouseEvent *ev) {
    MainWindow::mousePressEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        this->m_pressPos = ev->pos();
        // 获取鼠标在那个区域
    }
}

void KuGouApp::mouseMoveEvent(QMouseEvent *event) {
    MainWindow::mouseMoveEvent(event);
    // 计算的鼠标移动偏移量, 就是鼠标全局坐标 - 减去点击时鼠标坐标
    QPoint point_offset = event->globalPosition().toPoint() - mousePs;
    if ((event->buttons() == Qt::LeftButton) && isPress) {
        if (mouse_press_region == kMousePositionMid) {
            if(ui->title_widget->geometry().contains(this->m_pressPos) ||
                ui->play_widget->geometry().contains(this->m_pressPos)) {
                    move(windowsLastPs + point_offset);
                }
        } else {
            // 其他部分 是拉伸窗口
            // 获取客户区
            QRect rect = geometry();
            switch (mouse_press_region) {
                // 左上角
                case kMousePositionLeftTop:
                    rect.setTopLeft(rect.topLeft() + point_offset);
                break;
                case kMousePositionTop:
                    rect.setTop(rect.top() + point_offset.y());
                break;
                case kMousePositionRightTop:
                    rect.setTopRight(rect.topRight() + point_offset);
                break;
                case kMousePositionRight:
                    rect.setRight(rect.right() + point_offset.x());
                break;
                case kMousePositionRightBottom:
                    rect.setBottomRight(rect.bottomRight() + point_offset);
                break;
                case kMousePositionBottom:
                    rect.setBottom(rect.bottom() + point_offset.y());
                break;
                case kMousePositionLeftBottom:
                    rect.setBottomLeft(rect.bottomLeft() + point_offset);
                break;
                case kMousePositionLeft:
                    rect.setLeft(rect.left() + point_offset.x());
                break;
                default:
                    break;
            }
            setGeometry(rect);
            mousePs = event->globalPosition().toPoint();
        }
    }
}

void KuGouApp::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor("#eef2ff"));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, 8, 8);
    painter.drawPath(path);
}

void KuGouApp::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->m_sizeGrip->move(this->width() - this->m_sizeGrip->width() - 3,
                           this->height() - this->m_sizeGrip->height() - 3);
    this->m_sizeGrip->raise();
    this->m_sizeGrip->setVisible(true);
}

bool KuGouApp::event(QEvent *event) {
    if(QEvent::HoverMove == event->type())//鼠标移动
    {
        QMouseEvent *ev = static_cast<QMouseEvent*>(event);
        this->mouseMoveEvent(ev);
    }

    return QWidget::event(event);
}

bool KuGouApp::eventFilter(QObject *watched, QEvent *event) {
    if(watched == ui->progressSlider) {
        if (event->type()==QEvent::MouseButtonPress)//判断类型
        {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
                if(this->m_player->source().isEmpty()) return false;
                //qDebug() << "触发点击";
                qint64 value = QStyle::sliderValueFromPosition(ui->progressSlider->minimum(),
                    ui->progressSlider->maximum(), mouseEvent->pos().x(),
                    ui->progressSlider->width());
                this->m_player->setPosition(value);
                if(!this->m_isPlaying)ui->play_or_pause_toolButton->clicked();
            }
        }
    }
    return MainWindow::eventFilter(watched, event);
}

void KuGouApp::on_recommend_toolButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_recommendForYou.get());
}

void KuGouApp::on_local_download_toolButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_localDownload.get());
}

void KuGouApp::on_play_or_pause_toolButton_clicked() {
    //如果未设置播放源就return
    if(this->m_player->source().isEmpty()) return;
    this->m_isPlaying = !this->m_isPlaying;
    if (this->m_isPlaying) {
        this->m_player->play();
        ui->play_or_pause_toolButton->setIcon(QIcon("://Res/playbar/pause.svg"));
    } else {
        this->m_player->pause();
        ui->play_or_pause_toolButton->setIcon(QIcon("://Res/playbar/play.svg"));
    }
}

void KuGouApp::setPlayMusic(const QUrl &url) {
    //qDebug()<<"播放 "<<url;
    this->m_player->pause();
    this->m_player->setSource(url);
    ui->progressSlider->setValue(0);
    this->m_player->play();
}

void KuGouApp::updateProcess() {
    //qDebug()<<"sliderMoved / sliderReleased : "<<ui->progressSlider->value();
    qint64 position = ui->progressSlider->value() * this->m_player->duration() / ui->progressSlider->maximum();
    this->m_player->setPosition(position);
    this->m_player->play();
}

void KuGouApp::updateSliderRange(qint64 duration) {
    ui->progressSlider->setMaximum(duration);
    //qDebug()<<"改变总时长";
    ui->duration_label->setText(QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss"));
}

void KuGouApp::onPlayMusic(const QUrl &url,const SongInfor& info) {
    this->m_songInfor = info;
    setPlayMusic(url);
}

void KuGouApp::on_min_toolButton_clicked() {
    //QRect startGeometry = this->geometry(); // 获取当前窗口的几何形状(正常状态)
    //QRect endGeometry = startGeometry;
    ////endGeometry.setWidth(0); // 设置目标宽度为0
    //endGeometry.setHeight(100); // 设置目标高度为0
    //this->m_animation->setDuration(1000); // 动画时长
    //this->m_animation->setStartValue(startGeometry);
    //this->m_animation->setEndValue(endGeometry);
    //this->m_animation->setEasingCurve(QEasingCurve::InOutQuad); // 设置动画曲线
    //this->m_animation->start(); // 启动动画
    //// 最小化窗口
    //connect(this->m_animation.get(), &QPropertyAnimation::finished, this, [&]() {this->showMinimized();});
    this->showMinimized();
}

void KuGouApp::on_max_toolButton_clicked() {
    if (m_isMaxScreen) {
        this->m_isMaxScreen = false;
        endGeometry = startGeometry; // 获取普通尺寸时的几何形状
        startGeometry = this->screen()->availableGeometry();
        //this->setGeometry(startGeometry); // 恢复前，我们先把它设置回最大化尺寸
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url("://Res/titlebar/maximize-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })";
        ui->max_toolButton->setMyIcon(QIcon("://Res/titlebar/maximize-black.svg"));
        this->m_animation->setDuration(500); // 设置动画持续时间
    } else {
        this->m_isMaxScreen = true;
        // 如果当前不是最大化状态，则目标是最大化
        startGeometry = this->geometry();
        endGeometry = this->screen()->availableGeometry(); // 获取屏幕的最大化尺寸
        this->m_maxBtnStyle = R"(QToolButton#max_toolButton {
                                background-color: rgba(255,255,255,0);
                                qproperty-icon: url("://Res/titlebar/resume-black.svg");
                                border-radius: 6px;
                                height: 30px;
                                width: 30px;
                                icon-size: 12px 12px;
                            })";
        ui->max_toolButton->setMyIcon(QIcon("://Res/titlebar/resume-black.svg"));
        this->m_animation->setDuration(400); // 设置动画持续时间
    }

   this->m_animation->setStartValue(startGeometry); // 动画开始时的窗口几何
   this->m_animation->setEndValue(endGeometry); // 动画结束时的窗口几何
   this->m_animation->setEasingCurve(QEasingCurve::InOutQuad); // 设置动画的缓动曲线

    // 开始动画
    this->m_animation->start();

    // 更新按钮样式
    ui->max_toolButton->setStyleSheet(this->m_maxBtnStyle);
}

void KuGouApp::on_close_toolButton_clicked() {
    this->close();
}

void KuGouApp::on_circle_toolButton_clicked() {
    m_isSingleCircle = !m_isSingleCircle;
    if (m_isSingleCircle) {
        qDebug()<<"单曲循环";
        this->m_player->setLoops(QMediaPlayer::Loops::Infinite);
        ui->circle_toolButton->setStyleSheet(
            R"(QToolButton{border-image:url('://Res/playbar/single-list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/single-list-loop-blue.svg');})");
        //mediaStatusConnection = connect(this->m_player.get(),&QMediaPlayer::mediaStatusChanged,this, [=](QMediaPlayer::MediaStatus status) {
        //    if (status == QMediaPlayer::EndOfMedia) {
        //        qDebug()<<"播放结束";
        //        // 当播放结束时，重新开始播放
        //        qDebug()<<"循环播放 ："<<this->m_isSingleCircle;
        //        this->m_player->setPosition(0);  // 设置到文件的开头
        //        this->m_player->play();
        //    }
        //});
    } else {
        qDebug()<<"播放一次";
        //if (mediaStatusConnection) {
        //    disconnect(mediaStatusConnection);
        //    mediaStatusConnection = QMetaObject::Connection(); // 重置连接
        //}
        this->m_player->setLoops(QMediaPlayer::Loops::Once);
        ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/list-loop-blue.svg');})");
    }
}


