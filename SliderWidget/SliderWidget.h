#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include<QWidget>
#include<memory>
#include <QFile>
#include<QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOption>
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/*此处重写的QWidget的唯一目的就是作为中转传递信号。。。。*/
class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr)
        :QWidget(parent)
        ,m_effect(std::make_unique<QGraphicsDropShadowEffect>(this))
    {

        this->setFixedSize(64, 200);
        this->setContentsMargins(0, 0, 0, 10);
        {
            //设置样式
            QFile file(GET_CURRENT_DIR + QStringLiteral("/slider.css"));
            if (file.open(QIODevice::ReadOnly)) {
                this->setStyleSheet(file.readAll());
            } else {
                qDebug() << "样式表打开失败QAQ";
                return;
            }
        }
        this->m_effect->setColor(QColor(80, 80, 80));
        this->m_effect->setOffset(0, 0);
        this->m_effect->setBlurRadius(30);
        this->setGraphicsEffect(this->m_effect.get());
    }
protected:
    void paintEvent(QPaintEvent *ev) override {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

signals:
    void noVolume(bool flag);
private:
    std::unique_ptr<QGraphicsDropShadowEffect>m_effect;
};

class SliderWidget : public QSlider {
    Q_OBJECT

public:
    explicit SliderWidget(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);
    const int getValue() const ;
protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void showEvent(QShowEvent *event) override;

public slots:
    void onNoVolume(bool flag);
signals:
    void noVolume(bool flag);
public:
    bool m_isPressing;
private:
    int m_minValue;
    int m_maxValue;
    int m_currentValue;

    MyWidget *m_parent;
};


#endif //SLIDERWIDGET_H
