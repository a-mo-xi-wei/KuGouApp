//
// Created by WeiWang on 24-10-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LocalDownload.h" resolved

#include "LocalDownload.h"
#include "ui_LocalDownload.h"


#include<QStandardPaths>
#include<QFileDialog>
#include<QMediaMetaData>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QStringList>
#include<QRandomGenerator>
#include<QRegularExpression>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))
//匹配是否有乱码
static QRegularExpression re(QStringLiteral("^[A-Za-z0-9\\p{Han}\\\\/\\-_\\*]+$"));

LocalDownload::LocalDownload(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::LocalDownload)
    , m_player(new QMediaPlayer(this))
    ,m_searchAction(new QAction(this))
{
    ui->setupUi(this);

    QFile file(GET_CURRENT_DIR + QStringLiteral("/local.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    //经过一段时间的搜索得到的结论，如果想不调用play()就获得元数据信息是不现实的，能做到的也就只有加载完成之后立马停止
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [=](const QMediaPlayer::MediaStatus& status) {
        if (status == QMediaPlayer::LoadedMedia) {
            qDebug()<<"媒体状态改变，加载完成";
            // 停止播放
            this->m_player->stop();
            //qDebug()<<"元数据加载完成";
            const QMediaMetaData data = this->m_player->metaData();
            for(auto val : data.keys()) {
                qDebug()<<val<<": "<<data.value(val).toString();
            }

            //获取标题
            auto title = data.value(QMediaMetaData::Title).toString();
            if(!re.match(title).hasMatch()) {
                title = QUrl::fromLocalFile(this->m_mediaPath).fileName();
                title = title.first(title.lastIndexOf('.'));
            }
            //获取歌手
            auto singer = data.value(QMediaMetaData::ContributingArtist).toString();
            if(!re.match(singer).hasMatch())singer = QStringLiteral("网络歌手");
            //获取封面
            auto cover = data.value(QMediaMetaData::ThumbnailImage).value<QPixmap>();
            if(cover.isNull()) {
                //qDebug()<<"封面为空";
                cover = QPixmap(QString("://Res/tablisticon/pix%1.png").arg(QRandomGenerator::global()->bounded(1,11)));
            }
            //获取时长
            const auto duration = data.value(QMediaMetaData::Duration).value<qint64>();
            //信息赋值
            SongInfor tempInformation;
            tempInformation.index = this->m_locationMusicVector.size()+1;//让他先加1
            tempInformation.cover = cover;
            tempInformation.songName = title;
            tempInformation.signer = singer;
            tempInformation.duration = QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss");
            tempInformation.mediaPath = this->m_mediaPath;
            tempInformation.addTime = QDateTime::currentDateTime();
            //判重（通过元数据信息）
            auto it = std::find(this->m_locationMusicVector.begin(),
            this->m_locationMusicVector.end(),tempInformation);
            if(it == this->m_locationMusicVector.end())this->m_locationMusicVector.emplace_back(tempInformation);
            else {
                qDebug()<<title<<"已存在，请勿重复插入";
                //加载下一首歌
                loadNextSong();
                return;
            }
            //向parent发送添加MediaPath的信号
            emit addSongInfo(tempInformation);
            //加载相关信息
            auto item = new MusicItemWidget(tempInformation, this);
            item->setFillColor(QColor(QStringLiteral("#B0EDF6")));
            item->setRadius(12);
            item->setInterval(1);
            auto index = tempInformation.index;// 捕获当前的 index
            connect(item, &MusicItemWidget::playRequest, this, [index, this] {
                emit playMusic(index);
            });
            dynamic_cast<QVBoxLayout*>(ui->local_song_list_widget->layout())->insertWidget(ui->local_song_list_widget->layout()->count() - 1, item);
            ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
            //加载下一首歌
            loadNextSong();
        }
    });

    init();
}

LocalDownload::~LocalDownload() {
    delete ui;
}

void LocalDownload::init() {
    auto layout = ui->local_song_list_widget->layout();
    layout->setSpacing(0);
    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->setContentsMargins(0,0,0,0);

    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral("://Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral("://Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral("://Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral("://Res/window/index_lab.svg")));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();

    ui->local_play_toolButton->setIcon(QIcon(QStringLiteral("://Res/tabIcon/play3-white.svg")));
    ui->local_add_toolButton->setIcon(QIcon(QStringLiteral("://Res/tabIcon/add-gray.svg")));
    ui->upload_toolButton->setIcon(QIcon(QStringLiteral("://Res/tabIcon/upload-cloud-gray.svg")));
    ui->sort_toolButton->setIcon(QIcon(QStringLiteral("://Res/tabIcon/sort-gray.svg")));

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon(QStringLiteral("://Res/titlebar/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false);  // 仅显示图标
    ui->local_search_lineEdit->addAction(this->m_searchAction,QLineEdit::TrailingPosition);
    ui->local_search_lineEdit->setWidth(150);

    //先直接往里面嵌入一首歌
    //this->m_songQueue.enqueue();
    this->m_mediaPath = QStringLiteral("qrc:/Res/audio/紫荆花盛开.mp3");
    this->m_player->setSource(QUrl(this->m_mediaPath));
    this->m_player->play();  // 触发状态改变信号，获取元数据信息
}

void LocalDownload::loadNextSong() {
    if (!m_songQueue.isEmpty()) {
        this->m_mediaPath = m_songQueue.dequeue();  // 取出队列中的下一首歌路径
        qDebug()<<"取出歌曲 ： "<<this->m_mediaPath<<"=================";

        // 在加载新媒体前，重置媒体状态
        this->m_player->stop();
        this->m_player->setSource(QUrl());  // 清空当前媒体源
        this->m_player->play();
        this->m_player->setSource(QUrl::fromLocalFile(this->m_mediaPath));
        //this->m_player->stop();
        this->m_player->play();  // 触发状态改变信号，获取元数据信息
    }
}

void LocalDownload::on_local_play_toolButton_clicked() {
    emit startPlay();
}

void LocalDownload::on_local_add_toolButton_clicked() {
    QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QStringList paths = QFileDialog::getOpenFileNames(this, QStringLiteral("添加音乐"), musicPath, "Music (*.mp3 *.aac *.wav)");
    if (paths.isEmpty())return;
    //QString fileName = QUrl::fromLocalFile(path).fileName();
    //qDebug() << "插入："<<paths.size()<<"条数据";
    for(auto& path : paths) {
        //qDebug()<<"添加歌曲 ："<<path;
        this->m_songQueue.enqueue(path);
    }
    this->loadNextSong();
}

void LocalDownload::on_local_music_pushButton_clicked() {
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->local_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_music_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->downloaded_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->local_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_video_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->downloaded_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloading_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
}

void LocalDownload::setPlayIndex(const int &index) {
    this->m_setPlayIndex = index;
    if(this->m_curPlatIndex == -1) {
        this->m_curPlatIndex = index;
        auto widget = dynamic_cast<MusicItemWidget*>(ui->local_song_list_widget->layout()->itemAt(index)->widget());
        widget->setPlayState(true);
    }
    else {
        if(this->m_setPlayIndex != this->m_curPlatIndex) {
            auto widget = dynamic_cast<MusicItemWidget*>(ui->local_song_list_widget->layout()->itemAt(this->m_curPlatIndex)->widget());
            widget->setPlayState(false);
            widget = dynamic_cast<MusicItemWidget*>(ui->local_song_list_widget->layout()->itemAt(this->m_setPlayIndex)->widget());
            widget->setPlayState(true);
            this->m_curPlatIndex = this->m_setPlayIndex;
        }
    }

}
