#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include<QWidget>
#include<memory>
#include <QSlider>
#include<QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOption>

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
       const QString style = R"(
                        QSlider::groove:vertical {background-color:#ecf1f7;border:none;border-radius:2px;width: 3px;}
                        QSlider::handle:vertical {background-color:#26a1ff;border:none;height:13px;border-radius:6px;margin:0px -5px;}
                        QSlider::add-page:vertical {background-color:#26a1ff;border:none;border-radius:2px;}
                        QSlider::sub-page:vertical {border:none;border-radius:2px;}
                        QLabel {border: none;background-color:#ecf1f7}
                        QWidget{background-color:#fcfeff;border:none;border-radius:6px;}
)";

        this->setStyleSheet(style);
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

    void showEvent(QShowEvent *event) override;

public slots:
    void onNoVolume(bool flag);
signals:
    void noVolume(bool flag);
private:
    int m_minValue;
    int m_maxValue;
    int m_currentValue;
    bool m_isPressing;
    MyWidget *m_parent;
};


#endif //SLIDERWIDGET_H
