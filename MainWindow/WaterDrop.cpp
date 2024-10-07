#include "WaterDrop.h"
#include <QPainter>
#include <QPainterPath>
#include<QVariantAnimation>

#define WATER_DROP_RADIUS 20

WaterDrop::WaterDrop(QWidget *parent)
	: QWidget(parent)
	, m_waterDropAnimation(new QVariantAnimation(this))
	, m_animationRadius(0)
{
	this->setFixedSize(QSize(WATER_DROP_RADIUS * 2, WATER_DROP_RADIUS *2));
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_DeleteOnClose);
	m_waterDropAnimation->setStartValue(0);
	m_waterDropAnimation->setEndValue(WATER_DROP_RADIUS);
	m_waterDropAnimation->setDuration(300);

	connect(m_waterDropAnimation, &QVariantAnimation::valueChanged, this, &WaterDrop::onRaduisChanged);
	connect(m_waterDropAnimation, &QVariantAnimation::finished, this, &WaterDrop::close);
	m_waterDropAnimation->start();
}

WaterDrop::~WaterDrop()
{
}

void WaterDrop::move(const QPoint &point)
{
	QPoint translatePoint = point - QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS);

	QWidget::move(translatePoint);
}

void WaterDrop::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(QColor(0, 187, 255,120-m_animationRadius*2));
	pen.setWidthF(3);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	QPainterPath hidePath;
	hidePath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS),
		m_animationRadius, m_animationRadius);

	painter.drawPath(hidePath);
}

void WaterDrop::onRaduisChanged(QVariant value)
{
	m_animationRadius = value.toInt();
	update();
}