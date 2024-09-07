#include "KuGouApp.h"
#include "ui_KuGouApp.h"
#include"TableWidget.h"
#include"MusicItemWidget.h"

#include<QMediaMetaData>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QFileDialog>
#include<QStandardPaths>
#include<QPainterPath>
#include<QPainter>
#include<QPixmap>
#include<QDir>
#include<QFile>
#include<QPoint>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QButtonGroup>

KuGouApp::KuGouApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KuGouApp)
    ,m_player(std::make_unique<QMediaPlayer>())
    ,m_audioOutput(std::make_unique<QAudioOutput>())
    ,m_menuBtnGroup(std::make_unique<QButtonGroup>())
{
    ui->setupUi(this);
    QFile file("://Res/style.css");
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    }
    else {qDebug()<<"样式表打开失败QAQ";return;}
    initUi();
    this->m_player->setAudioOutput(this->m_audioOutput.get());
    connect(this->m_player.get(),&QMediaPlayer::positionChanged,this,&KuGouApp::updateSliderPosition);
    connect(this->m_player.get(),&QMediaPlayer::durationChanged,this,&KuGouApp::updateSliderRange);
    connect(this->m_player.get(),&QMediaPlayer::metaDataChanged,this,[this]{
        QMediaMetaData data = this->m_player->metaData();

        QString title = data.value(QMediaMetaData::Title).toString();
        QPixmap cover = data.value(QMediaMetaData::ThumbnailImage).value<QPixmap>();

        ui->cover_label->setPixmap(cover);
        ui->song_name_label->setText(title);
    });
}

KuGouApp::~KuGouApp()
{
    delete ui;
}

void KuGouApp::on_title_music_pushButton_clicked()
{
    ui->index_label1->show();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();
}

void KuGouApp::on_title_live_pushButton_clicked()
{
    ui->index_label1->hide();
    ui->index_label2->show();
    ui->index_label3->hide();
    ui->index_label4->hide();
    
}

void KuGouApp::on_title_listenBook_pushButton_clicked()
{
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->show();
    ui->index_label4->hide();
    
}

void KuGouApp::on_title_found_pushButton_clicked()
{
    ui->index_label1->hide();
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->show();
}

void KuGouApp::initUi()
{
    this->setWindowIcon(QIcon("://Res/window/windowIcon.svg"));
    //去掉标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    move(200, 200);
    this->setMouseTracking(true);
    ui->title_widget->setMouseTracking(true);
    ui->center_widget->setMouseTracking(true);
    ui->play_widget->setMouseTracking(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    initTitleWidget();
    initPoster();
    initTabWidget();
    initPlayWidget();
    initMenu();
    initLocalDownload();
}

QPixmap roundedPixmap(const QPixmap& src, QSize size, int radius) {
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

void KuGouApp::initTitleWidget()
{
    ui->index_label1->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30,15,Qt::KeepAspectRatio));
    ui->index_label2->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30,15,Qt::KeepAspectRatio));
    ui->index_label3->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30,15,Qt::KeepAspectRatio));
    ui->index_label4->setPixmap(QPixmap("://Res/titlebar/h-line.png").scaled(30,15,Qt::KeepAspectRatio));
    ui->index_label2->hide();
    ui->index_label3->hide();
    ui->index_label4->hide();
    ui->idx1_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx2_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx3_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx4_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();

    ui->search_lineEdit->addAction(QIcon("://Res/titlebar/search-black.svg"), QLineEdit::LeadingPosition);
    //除非自定义QToolButton否则达不到CSS中border-image的效果
    //ui->listen_toolButton->setIcon(QIcon("://Res/titlebar/listen-music-black.svg"));
    
    QPixmap rounded = roundedPixmap(QPixmap("://Res/window/portrait.jpg"), ui->title_portrait_label->size(), 20);  // 设置圆角半径
    ui->title_portrait_label->setPixmap(rounded);

    ui->title_gender_label->setPixmap(QPixmap("://Res/window/boy.svg"));
}

void KuGouApp::initPoster()
{
    QDir dir(__FILE__);
    if(dir.cdUp())dir.cd("Res/poster");

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    for(auto i = 1 ; i <= s ; ++i)
        ui->advertise_board_widget->addPoster(QPixmap(QString(":///Res/poster/%1.jpg").arg(i)));

}

void KuGouApp::initTabWidget()
{
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->table_widget->layout());
    if(!layout)return;
    layout->insertWidget(layout->count() -1 ,new TableWidget("潮流音乐站",this));
    layout->insertWidget(layout->count() -1 ,new TableWidget("热门好歌精选",this));
    layout->insertWidget(layout->count() -1 ,new TableWidget("私人专属好歌",this));

}

void KuGouApp::initPlayWidget()
{
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
    
    //ui->volume_toolButton->setIcon(QIcon("://Res/playbar/volume-on-gray.svg"));
    //ui->circle_toolButton->setIcon(QIcon("://Res/playbar/list-loop.svg"));

    ui->volume_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/volume-on-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/volume-on-blue.svg');})");

    ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/list-loop-blue.svg');})");

    ui->cover_label->setStyleSheet(R"(QLabel{border-image:url('://Res/playbar/default-cover-gray.svg');}
                                    QLabel:hover{border-image:url('://Res/playbar/default-cover-blue.svg');})");

}

void KuGouApp::initMenu()
{
    ui->basic_toolButton->setIcon(QIcon("://Res/window/recommend.svg"));
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

    m_menuBtnGroup->addButton(ui->basic_toolButton);
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

void KuGouApp::initLocalDownload()
{
    ui->local_play_toolButton->setIcon(QIcon("://Res/tabIcon/play3-white.svg"));
    ui->local_add_toolButton->setIcon(QIcon("://Res/tabIcon/add-gray.svg"));
    ui->upload_toolButton->setIcon(QIcon("://Res/tabIcon/upload-cloud-white.svg"));
    ui->search_toolButton->setIcon(QIcon("://Res/titlebar/search-black.svg"));
    ui->sort_toolButton->setIcon(QIcon("://Res/tabIcon/sort-gray.svg"));
    ui->local_list_toolButton->setIcon(QIcon("://Res/titlebar/menu-black.svg"));
    ui->local_zhuanji_toolButton->setIcon(QIcon("://Res/window/music-library.svg"));
    ui->local_singer_toolButton->setIcon(QIcon("://Res/window/ai.svg"));
    ui->local_file_toolButton->setIcon(QIcon("://Res/window/file.svg"));

}

void KuGouApp::mousePressEvent(QMouseEvent *ev)
{
    QWidget::mousePressEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        // 如果是鼠标左键
        // 获取当前窗口位置,以窗口左上角
        windowsLastPs = pos();
        // 获取鼠标在屏幕的位置  就是全局的坐标 以屏幕左上角为坐标系
        mousePs = ev->globalPosition().toPoint();
        isPress = true;
        // 获取鼠标在那个区域
        mouse_press_region = GetMouseRegion(ev->pos().x(), ev->pos().y());

    }
}

void KuGouApp::mouseReleaseEvent(QMouseEvent *ev)
{
    QWidget::mouseReleaseEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        isPress = false;
    }
    setCursor(QCursor{});
}

void KuGouApp::mouseMoveEvent(QMouseEvent *ev)
{
    QWidget::mouseMoveEvent(ev);
    // 设置鼠标的形状
    SetMouseCursor(ev->pos().x(), ev->pos().y());
    // 计算的鼠标移动偏移量, 就是鼠标全局坐标 - 减去点击时鼠标坐标
    QPoint point_offset = ev->globalPosition().toPoint() - mousePs;
    if ((ev->buttons() == Qt::LeftButton) && isPress)
    {
        if (mouse_press_region == kMousePositionMid)
        {
            // 如果鼠标是在窗口的中间位置,就是移动窗口
            move(windowsLastPs + point_offset);
        }
        else {
            // 其他部分 是拉伸窗口
            // 获取客户区
            QRect rect = geometry();
            switch (mouse_press_region)
            {
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
            mousePs = ev->globalPosition().toPoint();
        }
    }
    
}

void KuGouApp::paintEvent(QPaintEvent* ev)
{
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor("#ddddee"));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, 20, 20);
    painter.drawPath(path);
    
}

void KuGouApp::SetMouseCursor(int x, int y)   
{
    // 鼠标形状对象
    Qt::CursorShape cursor{};
    int region = GetMouseRegion(x, y);
    switch (region)
    {
    case kMousePositionLeftTop:
    case kMousePositionRightBottom:
        cursor = Qt::SizeFDiagCursor; break;
    case kMousePositionRightTop:
    case kMousePositionLeftBottom:
        cursor = Qt::SizeBDiagCursor; break;
    case kMousePositionLeft:
    case kMousePositionRight:
        cursor = Qt::SizeHorCursor; break;
    case kMousePositionTop:
    case kMousePositionBottom:
        cursor = Qt::SizeVerCursor; break;
    case kMousePositionMid:
        cursor = Qt::ArrowCursor; break;
    default:
        break;
    }
    setCursor(cursor);
}

int KuGouApp::GetMouseRegion(int x, int y)
{
    int region_x = 0, region_y = 0;
    // 鼠标的X坐标小于 边界5 说明他在最上层区域 第一区域
    if (x < kMouseRegionLeft)
    {
        region_x = 1;
    }
    else if (x > (this->width()/*窗体宽度*/ - kMouseRegionRight/*边界宽度5*/)) {
        // 如果鼠标X坐标 大于 最右侧的边界 说明他在第三区域
        region_x = 3;
    }
    else {
        region_x = 2;
    }
    if (y < kMouseRegionTop)
    {
        // 同理 鼠标Y坐标 小于上层边界5  说明鼠标在第一区域
        region_y = 1;
    }
    else if (y > (this->height() - kMouseRegionBottom)) {
        // 鼠标Y坐标的 大于 最下面的坐标,鼠标就在 第三区
        region_y = 3;
    }
    else {
        region_y = 2;
    }
    // 最后计算 表示区域的 数值 (x=1, y=1) 计算 = 1*10+1 =11
    // x=2,y=3 = 3*10+2 = 32 在图的 3,2 区域
    return region_y * 10 + region_x;
}

void KuGouApp::on_basic_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->main_page);
}

void KuGouApp::on_local_download_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->local_download_page);
}

void KuGouApp::on_local_add_toolButton_clicked() // 此处后面需要优化 ， 即使批量添加也没问题才行
{
    QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QString path = QFileDialog::getOpenFileName(this,"添加音乐",musicPath,"Music (*.mp3 *.aac *.wav)");
    if(path.isEmpty()){
        return;
    }
    this->m_locationMusicMap[this->m_locationMusicMap.size() + 1] = QUrl::fromLocalFile(path);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->local_song_list_widget->layout());
    if(!layout)return;
    int index = this->m_locationMusicMap.size();
    QString fileName = this->m_locationMusicMap[index].fileName();

    auto item = new MusicItemWidget(index,fileName.remove(fileName.lastIndexOf("."),fileName.length()),this);
    connect(item,&MusicItemWidget::playRequest,this,[this](int index){
        setPlayMusic(this->m_locationMusicMap[index]);
        on_play_or_pause_toolButton_clicked();
    });

    layout->insertWidget(layout->count() - 1,item);
    ui->local_music_number_label->setText(QString::number(this->m_locationMusicMap.size()));
}

void KuGouApp::on_play_or_pause_toolButton_clicked()
{
    this->m_isPlaying = !this->m_isPlaying;
    if(this->m_isPlaying){
        this->m_player->play();
        ui->play_or_pause_toolButton->setIcon(QIcon(":///Res/playbar/pause.svg"));
    }
    else {
        this->m_player->pause();
        ui->play_or_pause_toolButton->setIcon(QIcon(":///Res/playbar/play.svg"));
    }

}

void KuGouApp::setPlayMusic(const QUrl &url)
{
    this->m_player->setSource(url);
    ui->progressSlider->setValue(0);

}

void KuGouApp::updateSliderPosition(qint64 position)
{
    ui->progressSlider->setValue(position);
}

void KuGouApp::updateSliderRange(qint64 duration)
{
    ui->progressSlider->setRange(0,duration);
}

void KuGouApp::on_min_toolButton_clicked()
{
    this->showMinimized();
}

void KuGouApp::on_max_toolButton_clicked()
{
    static bool is_max = false;
    if(is_max)this->showNormal();
    else this->showMaximized();
    is_max = !is_max;
}

void KuGouApp::on_close_toolButton_clicked()
{
    this->close();
}

void KuGouApp::on_circle_toolButton_clicked()
{
    isSingelCircle = !isSingelCircle;
    if(isSingelCircle){
        ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/single-list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/single-list-loop-blue.svg');})");

    }
    else{
        ui->circle_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/list-loop-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/list-loop-blue.svg');})");
    }
}

void KuGouApp::on_volume_toolButton_clicked()
{
    isNoVolume = !isNoVolume;
    if(isNoVolume){
        ui->volume_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/volume-off-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/volume-off-blue.svg');})");

    }
    else{
        ui->volume_toolButton->setStyleSheet(R"(QToolButton{border-image:url('://Res/playbar/volume-on-gray.svg');}
                                            QToolButton:hover{border-image:url('://Res/playbar/volume-on-blue.svg');})");
    }
}
