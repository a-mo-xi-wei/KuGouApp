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
KuGouApp::KuGouApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KuGouApp)
    ,m_player(std::make_unique<QMediaPlayer>())
    ,m_audioOutput(std::make_unique<QAudioOutput>())
{
    ui->setupUi(this);

    QFile file(":/style.css");
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    }

    initTitleWidget();
    initPoster();
    initTabWidget();
    initPlayWidget();
    initMenu();
    initLocalDownload();

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
    ui->search_lineEdit->addAction(QIcon(":/image/titlebar/search-black.svg"), QLineEdit::LeadingPosition);
    //除非自定义QToolButton否则达不到CSS中的效果
    //ui->listen_toolButton->setIcon(QIcon(":/image/titlebar/listen-music-black.svg"));
    
    QPixmap rounded = roundedPixmap(QPixmap(":/image/window/portrait.jpg"), ui->portrait_label->size(), 20);  // 设置圆角半径
    ui->portrait_label->setPixmap(rounded);

    ui->gender_label->setPixmap(QPixmap(":/image/window/boy.svg"));
}

void KuGouApp::initPoster()
{
    QDir dir(__FILE__);
    if(dir.cdUp())dir.cd("image/poster");

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    for(auto i = 1 ; i <= s ; ++i)
        ui->advertise_board_widget->addPoster(QPixmap(QString("://image/poster/%1.jpg").arg(i)));
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
    ui->love_toolButton->setIcon(QIcon(":/image/playbar/collect.svg"));
    ui->download_toolButton->setIcon(QIcon(":/image/playbar/download.svg"));
    ui->comment_toolButton->setIcon(QIcon(":/image/playbar/comment.svg"));
    ui->share_toolButton->setIcon(QIcon(":/image/playbar/share.svg"));
    ui->more_toolButton->setIcon(QIcon(":/image/playbar/more.svg"));
    ui->pre_toolButton->setIcon(QIcon(":/image/playbar/previous-song.svg"));
    ui->play_or_pause_toolButton->setIcon(QIcon(":/image/playbar/play.svg"));
    ui->next_toolButton->setIcon(QIcon(":/image/playbar/next-song.svg"));
    ui->volume_toolButton->setIcon(QIcon(":/image/playbar/volume.svg"));
    ui->circle_toolButton->setIcon(QIcon(":/image/playbar/list-loop.svg"));
    ui->erji_toolButton->setIcon(QIcon(":/image/playbar/together.svg"));
    ui->ci_toolButton->setIcon(QIcon(":/image/playbar/song-words.svg"));
    ui->list_toolButton->setIcon(QIcon(":/image/playbar/play-list.svg"));
    
}

void KuGouApp::initMenu()
{
    ui->basic_toolButton->setIcon(QIcon(":/image/window/recommend.svg"));
    ui->yueku_toolButton->setIcon(QIcon(":/image/window/music-library.svg"));
    ui->gedan_toolButton->setIcon(QIcon(":/image/window/song-list.svg"));
    ui->pindao_toolButton->setIcon(QIcon(":/image/window/my-channel.svg"));
    ui->video_toolButton->setIcon(QIcon(":/image/window/video.svg"));
    ui->live_toolButton->setIcon(QIcon(":/image/window/live.svg"));
    ui->my_shoucang_toolButton->setIcon(QIcon(":/image/window/collect.svg"));
    ui->my_pindao_toolButton->setIcon(QIcon(":/image/window/my-channel.svg"));
    ui->local_download_toolButton->setIcon(QIcon(":/image/window/download.svg"));
    ui->music_yunpan_toolButton->setIcon(QIcon(":/image/window/cloud.svg"));
    ui->yigou_yunpan_toolButton->setIcon(QIcon(":/image/window/bought.svg"));
    ui->zuijin_bofang_toolButton->setIcon(QIcon(":/image/window/history.svg"));
    ui->moren_liebiao_toolButton->setIcon(QIcon(":/image/titlebar/menu-black.svg"));
}

void KuGouApp::initLocalDownload()
{
    ui->local_play_toolButton->setIcon(QIcon(":/image/tabIcon/play3-white.svg"));
    ui->batch_operation_toolButton->setIcon(QIcon(":/image/tabIcon/batch-operation-gray.svg"));
    ui->local_add_toolButton->setIcon(QIcon(":/image/tabIcon/add-gray.svg"));
    ui->upload_toolButton->setIcon(QIcon(":/image/tabIcon/upload-cloud-white.svg"));
    ui->search_toolButton->setIcon(QIcon(":/image/titlebar/search-black.svg"));
    ui->sort_toolButton->setIcon(QIcon(":/image/tabIcon/sort-gray.svg"));
    ui->local_list_toolButton->setIcon(QIcon(":image/titlebar/menu-black.svg"));
    ui->local_zhuanji_toolButton->setIcon(QIcon(":/image/window/music-library.svg"));
    ui->local_singer_toolButton->setIcon(QIcon(":/image/window/ai.svg"));
    ui->local_file_toolButton->setIcon(QIcon(":/image/window/file.svg"));

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
        ui->play_or_pause_toolButton->setIcon(QIcon("://image/playbar/pause.svg"));
    }
    else {
        this->m_player->pause();
        ui->play_or_pause_toolButton->setIcon(QIcon("://image/playbar/play.svg"));
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

