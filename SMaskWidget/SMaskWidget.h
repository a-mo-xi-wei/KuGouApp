#ifndef SMASKWIDGET_H
#define SMASKWIDGET_H

#include <QWidget>
class QPaintEvent;

class SMaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit SMaskWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event)override;
};

#endif // SMASKWIDGET_H
