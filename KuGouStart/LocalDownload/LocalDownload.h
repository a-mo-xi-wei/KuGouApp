//
// Created by WeiWang on 24-10-10.
//

#ifndef LOCALDOWNLOAD_H
#define LOCALDOWNLOAD_H

#include"MusicItemWidget.h"

#include<QQueue>
#include<QWidget>
class QMediaPlayer;

QT_BEGIN_NAMESPACE
namespace Ui { class LocalDownload; }
QT_END_NAMESPACE

class LocalDownload : public QWidget {
Q_OBJECT

public:
    explicit LocalDownload(QWidget *parent = nullptr);
    ~LocalDownload() override;
    void init();
    void loadNextSong();
private slots:
    void on_local_play_toolButton_clicked();

    void on_local_add_toolButton_clicked();

    void on_local_music_pushButton_clicked();

    void on_downloaded_music_pushButton_clicked();

    void on_downloaded_video_pushButton_clicked();

    void on_downloading_pushButton_clicked();

signals:
    void playMusic(const int& index);

    void startPlay();

    void addSongInfo(const SongInfor& info);

public slots:
    void setPlayIndex(const int& index);

private:
    Ui::LocalDownload *ui;
    QMediaPlayer* m_player{};
    QVector<SongInfor> m_locationMusicVector;

    QAction* m_searchAction;//专门为了设置图片
    QString m_mediaPath;
    QQueue<QString> m_songQueue;

    //固定颜色专用
    int m_curPlatIndex = -1;
    int m_setPlayIndex = -1;
};


#endif //LOCALDOWNLOAD_H
