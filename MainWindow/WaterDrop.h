#ifndef WATERDROP_H
#define WATERDROP_H

#include <QWidget>
class QVariantAnimation;

class WaterDrop : public QWidget
{
    Q_OBJECT

public:
    WaterDrop(QWidget *parent = Q_NULLPTR);

	~WaterDrop();

	void move(const QPoint &point);

protected:

	void paintEvent(QPaintEvent *event)override;

public slots:
	void onRaduisChanged(QVariant value);

private:
	QVariantAnimation* m_waterDropAnimation;

	int m_animationRadius;
};
#endif // WATERDROP_H