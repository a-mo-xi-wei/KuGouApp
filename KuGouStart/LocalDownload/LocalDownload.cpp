//
// Created by WeiWang on 24-10-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LocalDownload.h" resolved

#include "LocalDownload.h"
#include "ui_LocalDownload.h"
#include"MusicItemWidget.h"

#include<QStandardPaths>
#include<QFileDialog>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\")))

LocalDownload::LocalDownload(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::LocalDownload)
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

    init();
}

LocalDownload::~LocalDownload() {
    delete ui;
}

void LocalDownload::init() {
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

    //先直接往里面嵌入两首歌
    //auto item = new MusicItemWidget(1, "紫荆花盛开", this);
    auto item = new MusicItemWidget(m_informationMap, this);
    connect(item, &MusicItemWidget::playRequest, this, [this](int index) {
        //qDebug()<<"发送播放歌曲信号 ";
        emit playMusic(QUrl("qrc:/Res/audio/紫荆花盛开.mp3"));
    });
    this->m_locationMusicVector.emplace_back("紫荆花盛开");
    ui->local_song_list_layout->insertWidget(ui->local_song_list_layout->count() - 1, item);
    //item = new MusicItemWidget(2, "青花瓷", this);
    item = new MusicItemWidget(m_informationMap, this);
    connect(item, &MusicItemWidget::playRequest, this, [this](int index) {
        emit playMusic(QUrl("qrc:/Res/audio/青花瓷.mp3"));
    });
    this->m_locationMusicVector.emplace_back("青花瓷");
    ui->local_song_list_layout->insertWidget(ui->local_song_list_layout->count() - 1, item);
    ui->local_music_number_label->setText("2");
}

void LocalDownload::on_local_add_toolButton_clicked() {
    QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QString path = QFileDialog::getOpenFileName(this, "添加音乐", musicPath, "Music (*.mp3 *.aac *.wav)");
    if (path.isEmpty())return;
    QString fileName = QUrl::fromLocalFile(path).fileName();
    auto it = std::find(this->m_locationMusicVector.begin(),
        this->m_locationMusicVector.end(),fileName);
    if(it != this->m_locationMusicVector.end())this->m_locationMusicVector.emplace_back();
    else {
        qDebug()<<"歌曲已存在，请勿重复插入";
        return;
    }

    //auto item = new MusicItemWidget(this->m_locationMusicVector.size(),
    //    fileName.remove(fileName.lastIndexOf("."), fileName.length()), this);
    auto item = new MusicItemWidget(m_informationMap, this);
    connect(item, &MusicItemWidget::playRequest, this, [this](int index) {
        emit playMusic(this->m_locationMusicVector[index]);
    });

    ui->local_song_list_layout->insertWidget(ui->local_song_list_layout->count() - 1, item);
    ui->local_music_number_label->setText(QString::number(this->m_locationMusicVector.size()));
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