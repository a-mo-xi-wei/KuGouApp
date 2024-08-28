#ifndef ADVERTISEBOARD_H
#define ADVERTISEBOARD_H

#include <QWidget>
#include<QList>
class QPixmap;
class QTimer;

class AdvertiseBoard : public QWidget
{
    Q_OBJECT
public:
    explicit AdvertiseBoard(QWidget *parent = nullptr);
    ~AdvertiseBoard();
public:
    void addPoster(const QPixmap& posterPix);

protected:
    void paintEvent(QPaintEvent* ev)override;

private:
    QList<QPixmap*>m_posters;
    QTimer* m_timer{};
};

#endif // ADVERTISEBOARD_H
