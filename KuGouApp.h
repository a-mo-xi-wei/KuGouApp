#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include <QWidget>
#include<memory>
#include<QMap>
#include<QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class KuGouApp;
}
QT_END_NAMESPACE

class QMediaPlayer;
class QAudioOutput;

class KuGouApp : public QWidget
{
    Q_OBJECT

public:
    KuGouApp(QWidget *parent = nullptr);
    ~KuGouApp();
private slots:
    void on_basic_toolButton_clicked();

    void on_local_download_toolButton_clicked();

    void on_local_add_toolButton_clicked();

    void on_play_or_pause_toolButton_clicked();
public slots:
    void setPLayMusic(const QUrl& url);
private:
    void initPoster();
    void initTabWidget();
private:
    Ui::KuGouApp *ui;
    std::unique_ptr<QMediaPlayer>m_player{};
    std::unique_ptr<QAudioOutput>m_audioOutput{};
    QMap<int,QUrl>m_locationMusicMap;
    bool m_isPlaying = false;
};
#endif // KUGOUAPP_H
