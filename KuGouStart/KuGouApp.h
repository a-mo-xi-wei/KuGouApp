#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include<memory>
#include<QUrl>
#include"MainWindow.h"
#include"RecommendForYou.h"
#include"LocalDownload.h"

class QMediaPlayer;
class QAudioOutput;
class QButtonGroup;
class QSizeGrip;
class QPropertyAnimation;

QT_BEGIN_NAMESPACE

namespace Ui {
    class KuGouApp;
}

QT_END_NAMESPACE

class KuGouApp : public MainWindow {
    Q_OBJECT

public:
    explicit KuGouApp(MainWindow *parent = nullptr);

    ~KuGouApp() override;

private slots:
    void on_title_return_toolButton_clicked();

    void on_title_refresh_toolButton_clicked();

    void on_title_music_pushButton_clicked();

    void on_title_live_pushButton_clicked();

    void on_title_listenBook_pushButton_clicked();

    void on_title_found_pushButton_clicked();

    void on_recommend_toolButton_clicked();

    void on_local_download_toolButton_clicked();

    void on_play_or_pause_toolButton_clicked();

    void on_min_toolButton_clicked();

    void on_max_toolButton_clicked();

    void on_close_toolButton_clicked();

    void on_circle_toolButton_clicked();


public slots:
    void setPlayMusic(const QUrl &url);

    void updateProcess();

    void updateSliderRange(qint64 duration);

    void onPlayMusic(const int& index);

    void onStartPlay();

    void onAddSongInfo(const SongInfor &info);

private:
    void initUi();

    void initCommendForYouWidget();

    void initLocalDownload();

    void initTitleWidget();

    void initPlayWidget();

    void initMenu();

    void initCornerWidget();

protected:
    void mousePressEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    bool event(QEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::KuGouApp *ui;
    std::unique_ptr<QMediaPlayer> m_player{};
    std::unique_ptr<QAudioOutput> m_audioOutput{};
    std::unique_ptr<QButtonGroup> m_menuBtnGroup{};
    std::unique_ptr<QSizeGrip> m_sizeGrip{};
    std::unique_ptr<QPropertyAnimation> m_animation{};
    std::unique_ptr<RecommendForYou> m_recommendForYou{};
    std::unique_ptr<LocalDownload> m_localDownload{};

    bool m_isSingleCircle = false;
    bool m_isMaxScreen = false;
    QMetaObject::Connection mediaStatusConnection;

    bool m_isPlaying = false;
    QPoint m_pressPos;
    QString m_maxBtnStyle;
    QRect startGeometry; // 获取当前窗口的几何形状(正常状态)
    QRect endGeometry;

    bool m_isOrderPlay = false;//专门提供给开始播放按钮
    int m_orderIndex = 0;
    QVector<SongInfor>m_songInfoVector;
    int m_songIndex;//播放的歌曲的下标
};
#endif // KUGOUAPP_H
