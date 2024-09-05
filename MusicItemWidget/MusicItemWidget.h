#ifndef MUSICITEMWIDGET_H
#define MUSICITEMWIDGET_H

#include <QWidget>

class QLabel;
class QToolButton;

class MusicItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicItemWidget(int index, const QString& name, QWidget *parent = nullptr);

signals:
    void playRequest(int indx);

private:
    void initUi();

private:
    QLabel* m_indexLab;
    QLabel* m_coverLab;
    QLabel* m_nameLab;
    QToolButton* m_playToolBtn;
    QToolButton* m_playNextToolBtn;
    QToolButton* m_downloadToolBtn;
    QToolButton* m_collectToolBtn;
    QToolButton* m_moreToolBtn;
    int m_index;
};

#endif // MUSICITEMWIDGET_H
