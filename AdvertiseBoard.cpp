#include "AdvertiseBoard.h"
#include<QPixmap>
#include<QTimer>
#include<QPaintevent>
#include<QPainter>
#include<QPainterPath>

static const int G_Radius = 5;  //圆球半径
static const int G_Space = 2 * G_Radius;   //圆球之间的间隔

AdvertiseBoard::AdvertiseBoard(QWidget *parent)
    : QWidget{parent}
    ,m_timer(new QTimer(this))
{
    connect(this->m_timer,&QTimer::timeout,this,qOverload<>(&AdvertiseBoard::update));
    this->m_timer->start(3000);

    addPoster(QPixmap("://image/poster/1.jpg"));
    addPoster(QPixmap("://image/poster/2.jpg"));
}

AdvertiseBoard::~AdvertiseBoard()
{
    for(auto& pix : this->m_posters){
        if(pix)delete pix;
    }
    this->m_posters.clear();
}

void AdvertiseBoard::addPoster(const QPixmap &posterPix)
{
    QPixmap* pix = new QPixmap(posterPix);
    this->m_posters.append(pix);
}

void AdvertiseBoard::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    if(this->m_posters.isEmpty()){
        QWidget::paintEvent(ev);
        return;
    }
    static int posterIndex = 0;
    QPixmap img = this->m_posters[posterIndex]->scaled(this->size(),Qt::KeepAspectRatio);
    //将图片变成圆角
    QPainterPath path;
    path.addRoundedRect(this->rect(),10,10);
    painter.setClipPath(path);

    //绘制广告图
    painter.drawPixmap(this->rect(),img);
    posterIndex = (++posterIndex) % this->m_posters.size();

    //绘制广告播放进度
    auto s = this->m_posters.size();
    auto drawStartPos = QPoint((this->width() - (s-1) * (G_Radius * 2 + G_Space)) / 2,this->height() - 10);
    for(size_t i = 0 ; i < this->m_posters.size() ; ++i){
        if(i == posterIndex)
            painter.setBrush(Qt::red);
        else
            painter.setBrush(Qt::white);
        painter.drawEllipse(drawStartPos,G_Radius,G_Radius);
        drawStartPos.setX(drawStartPos.x() + G_Space + 2 * G_Radius);
    }
    QWidget::paintEvent(ev);
}
