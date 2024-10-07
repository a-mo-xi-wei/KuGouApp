#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include<memory>
#include<QMap>
#include<QUrl>
#include"MainWindow.h"

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
    KuGouApp(MainWindow *parent = nullptr);

    ~KuGouApp();

    QPoint getVolumeBtnPos() const;

private slots:
    void on_title_return_toolButton_clicked();

    void on_title_refresh_toolButton_clicked();

    void on_title_music_pushButton_clicked();

    void on_title_live_pushButton_clicked();

    void on_title_listenBook_pushButton_clicked();

    void on_title_found_pushButton_clicked();

    void on_basic_toolButton_clicked();

    void on_local_download_toolButton_clicked();

    void on_local_add_toolButton_clicked();

    void on_play_or_pause_toolButton_clicked();

    void on_min_toolButton_clicked();

    void on_max_toolButton_clicked();

    void on_close_toolButton_clicked();

    void on_circle_toolButton_clicked();

    void on_local_music_pushButton_clicked();

    void on_downloaded_music_pushButton_clicked();

    void on_downloaded_video_pushButton_clicked();

    void on_downloading_pushButton_clicked();

public slots:
    void setPlayMusic(const QUrl &url);

    void updatePositionLab(int position);

    void updateSliderRange(int duration);

    void updateSliderPosition();
private:
    void initUi();

    void initTitleWidget();

    void initAdvertiseBoard();

    void initTabWidget();

    void initPlayWidget();

    void initMenu();

    void initLocalDownload();

    void initBottomWidget();

private:
    Ui::KuGouApp *ui;
    std::unique_ptr<QMediaPlayer> m_player{};
    std::unique_ptr<QAudioOutput> m_audioOutput{};
    std::unique_ptr<QButtonGroup> m_menuBtnGroup{};
    std::unique_ptr<QSizeGrip> m_sizeGrip{};
    std::unique_ptr<QPropertyAnimation> m_animation{};
    QMap<int, QUrl> m_locationMusicMap;
    bool m_isPlaying = false;
    QPoint m_pressPos;
    QString m_maxBtnStyle;
    QRect startGeometry; // 获取当前窗口的几何形状(正常状态)
    QRect endGeometry;
protected:
    void mousePressEvent(QMouseEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *ev) override;

    void resizeEvent(QResizeEvent *event) override;

    bool event(QEvent *event) override;

private:
    bool m_isSingleCircle = false;
    bool m_isMaxScreen = false;

};
#endif // KUGOUAPP_H
