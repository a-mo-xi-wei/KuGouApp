#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QPaintEvent>

class TitleWidget : public QWidget {
    Q_OBJECT

public:
    explicit TitleWidget(QWidget *parent = nullptr);
signals:
    void doubleClicked();  // 自定义的双击信号

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override ;
    void paintEvent(QPaintEvent* ev)override;
};

#endif // TITLEWIDGET_H
