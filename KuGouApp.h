#ifndef KUGOUAPP_H
#define KUGOUAPP_H

#include <QWidget>

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

private:
    Ui::KuGouApp *ui;
};
#endif // KUGOUAPP_H
