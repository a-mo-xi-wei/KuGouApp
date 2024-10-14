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
#define GET_CURRENT_DIR (std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\")))
//匹配是否有乱码
static QRegularExpression re("^[A-Za-z0-9\\p{Han}\\\\/\\-_\\*]+$");

LocalDownload::LocalDownload(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::LocalDownload)
    , m_player(std::make_unique<QMediaPlayer>(this))
    ,m_searchAction(new QAction(this))
{
    ui->setupUi(this);

    QFile file((GET_CURRENT_DIR + "/local.css").data());
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        return;
    }
    //经过一段时间的搜索得到的结论，如果想不调用play()就获得元数据信息是不现实的，能做到的也就只有加载完成之后立马停止
    connect(m_player.get(), &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            // 停止播放
            this->m_player->stop();
            //qDebug()<<"元数据加载完成";
            const QMediaMetaData data = this->m_player->metaData();
            //for(auto val : data.keys()) {
            //    qDebug()<<val<<": "<<data.value(val).toString();
            //}

            //获取标题
            auto title = data.value(QMediaMetaData::Title).toString();
            if(!re.match(title).hasMatch()) {
                title = QUrl::fromLocalFile(this->m_mediaPath).fileName();
                title = title.mid(0,title.lastIndexOf('.'));
            }
            //判重（仅通过标题）
            auto it = std::find(this->m_locationMusicVector.begin(),
            this->m_locationMusicVector.end(),title);
            if(it == this->m_locationMusicVector.end())this->m_locationMusicVector.emplace_back(title);
            else {
                //qDebug()<<title<<"已存在，请勿重复插入";
                return;
            }
            //获取歌手
            auto singer = data.value(QMediaMetaData::ContributingArtist).toString();
            if(!re.match(singer).hasMatch())singer = "网络歌手";
            //获取封面
            auto cover = data.value(QMediaMetaData::ThumbnailImage).value<QPixmap>();
            if(cover.isNull()) {
                //qDebug()<<"封面为空";
                cover = QPixmap(QString("://Res/tablisticon/pix%1.png").arg(QRandomGenerator::global()->bounded(1,11)));
            }
            //获取时长
            const auto duration = data.value(QMediaMetaData::Duration).value<qint64>();

            //信息赋值
            this->m_information.index = this->m_locationMusicVector.size();
            this->m_information.cover = cover;
            this->m_information.songName = title;
            this->m_information.signer = singer;
            this->m_information.duration = QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss");
            this->m_information.mediaPath = this->m_mediaPath;

            //加载相关信息
            auto item = new MusicItemWidget(m_information, this);
            item->setFillColor(QColor("#B0EDF6"));
            item->setRadius(10);
            item->setInitval(1);
            SongInfor info = this->m_information;// 捕获当前的 m_information
            connect(item, &MusicItemWidget::playRequest, this, [info, this] {
                emit playMusic(info);
            });
            //qDebug()<<"添加 "<<m_information.songName<<" :"<<m_information.signer<<" 成功";
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
    ui->idx1_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx2_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx3_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx4_lab->setPixmap(QPixmap("://Res/window/index_lab.svg"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();

    ui->local_play_toolButton->setIcon(QIcon("://Res/tabIcon/play3-white.svg"));
    ui->local_add_toolButton->setIcon(QIcon("://Res/tabIcon/add-gray.svg"));
    ui->upload_toolButton->setIcon(QIcon("://Res/tabIcon/upload-cloud-gray.svg"));
    ui->sort_toolButton->setIcon(QIcon("://Res/tabIcon/sort-gray.svg"));

    //使用 addAction 添加右侧图标
    this->m_searchAction->setIcon(QIcon("://Res/titlebar/search-black.svg"));
    this->m_searchAction->setIconVisibleInMenu(false);  // 仅显示图标
    ui->local_search_lineEdit->addAction(this->m_searchAction,QLineEdit::TrailingPosition);
    ui->local_search_lineEdit->setWidth(150);

    //先直接往里面嵌入一首歌
    this->m_songQueue.enqueue("qrc:/Res/audio/紫荆花盛开.mp3");
    //this->m_songQueue.enqueue("qrc:/Res/audio/青花瓷.mp3");

    this->loadNextSong();
}

void LocalDownload::loadNextSong() {
    if (!m_songQueue.isEmpty()) {
        this->m_mediaPath = m_songQueue.dequeue();  // 取出队列中的下一首歌路径
        //qDebug()<<this->m_mediaPath<<"=================";
        this->m_player->setSource(QUrl(this->m_mediaPath));
        this->m_player->play();  // 触发状态改变信号，获取元数据信息
    }
}

void LocalDownload::on_local_play_toolButton_clicked() {
    emit startPlay();
}

void LocalDownload::on_local_add_toolButton_clicked() {
    QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QStringList paths = QFileDialog::getOpenFileNames(this, "添加音乐", musicPath, "Music (*.mp3 *.aac *.wav)");
    if (paths.isEmpty())return;
    //QString fileName = QUrl::fromLocalFile(path).fileName();
    //qDebug() << "插入："<<paths.size()<<"条数据";
    for(auto& path : paths)this->m_songQueue.enqueue(path);
    this->loadNextSong();
}

void LocalDownload::on_local_music_pushButton_clicked() {
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->local_music_number_label->setStyleSheet("color:#26a1ff;font-size:16px;font-weight:bold;");
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_music_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->downloaded_music_number_label->setStyleSheet("color:#26a1ff;font-size:16px;font-weight:bold;");
    ui->local_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_video_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->downloaded_video_number_label->setStyleSheet("color:#26a1ff;font-size:16px;font-weight:bold;");
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloading_pushButton_clicked() {
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->downloading_number_label->setStyleSheet("color:#26a1ff;font-size:16px;font-weight:bold;");
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
}
