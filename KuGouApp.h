#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include <QWidget>
#include<memory>
#include<QMap>
#include<QUrl>

class QMediaPlayer;
class QAudioOutput;


namespace {
/* 这里我们将一个窗口划分为9个区域，分别为
 左上角（1, 1）、中上（1，2）、右上角（1, 3）
 左中  （2, 1）、 中间（2, 2）、右中  （2, 3）
 左下角（3, 1）、中下（3，2）、 右下角（3, 3）*/
const int kMouseRegionLeft = 10;

const int kMouseRegionTop = 10;

const int kMouseRegionRight = 10;

const int kMouseRegionBottom = 10;

}

// 鼠标的 活动范围的 枚举
enum MousePosition

{
    // 这个是上面图片划分的区域 1,1 区 就用 11 代表 , 1,2 就用12 代表 以此类推
    kMousePositionLeftTop = 11,

    kMousePositionTop = 12,

    kMousePositionRightTop = 13,

    kMousePositionLeft = 21,

    kMousePositionMid = 22,

    kMousePositionRight = 23,

    kMousePositionLeftBottom = 31,

    kMousePositionBottom = 32,

    kMousePositionRightBottom = 33,

};

QT_BEGIN_NAMESPACE
namespace Ui {
class KuGouApp;
}
QT_END_NAMESPACE


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

    void on_min_toolButton_clicked();

    void on_max_toolButton_clicked();

    void on_close_toolButton_clicked();

public slots:
    void setPlayMusic(const QUrl& url);
    void updateSliderPosition(qint64 position);
    void updateSliderRange(qint64 duration);

private:
    void initUi();
    void initTitleWidget();
    void initPoster();
    void initTabWidget();
    void initPlayWidget();
    void initMenu();
    void initLocalDownload();
private:
    Ui::KuGouApp *ui;
    std::unique_ptr<QMediaPlayer>m_player{};
    std::unique_ptr<QAudioOutput>m_audioOutput{};
    QMap<int,QUrl>m_locationMusicMap;
    bool m_isPlaying = false;
protected:
    void mousePressEvent(QMouseEvent* ev)override;
    void mouseReleaseEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* event)override;
    void paintEvent(QPaintEvent* ev)override;
private:
    //根据鼠标的设置鼠标样式，用于拉伸
    void SetMouseCursor(int x, int y);
    //判断鼠标的区域，用于拉伸
    int GetMouseRegion(int x, int y);
private:
    bool isPress = false;
    QPoint windowsLastPs;
    QPoint mousePs;
    int mouse_press_region = kMousePositionMid;
};
#endif // KUGOUAPP_H
