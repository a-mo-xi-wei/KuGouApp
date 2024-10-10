//
// Created by WeiWang on 24-10-10.
//

#ifndef LOCALDOWNLOAD_H
#define LOCALDOWNLOAD_H

#include <QWidget>
#include<QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class LocalDownload; }
QT_END_NAMESPACE

class LocalDownload : public QWidget {
Q_OBJECT

public:
    explicit LocalDownload(QWidget *parent = nullptr);
    ~LocalDownload() override;
    void init();

private slots:

    void on_local_add_toolButton_clicked();

    void on_local_music_pushButton_clicked();

    void on_downloaded_music_pushButton_clicked();

    void on_downloaded_video_pushButton_clicked();

    void on_downloading_pushButton_clicked();
signals:
    void playMusic(const QUrl &url);
private:
    Ui::LocalDownload *ui;
    QVector<QString> m_locationMusicVector;
    QMap<QString, QString> m_informationMap;
    QAction* m_searchAction;
};


#endif //LOCALDOWNLOAD_H
