#include "KuGouApp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KuGouApp w;
    w.show();
    return a.exec();
}
