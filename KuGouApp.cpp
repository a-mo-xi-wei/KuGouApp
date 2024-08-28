#include "KuGouApp.h"
#include "ui_KuGouApp.h"

KuGouApp::KuGouApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KuGouApp)
{
    ui->setupUi(this);
}

KuGouApp::~KuGouApp()
{
    delete ui;
}

void KuGouApp::initPoster()
{
    auto w ;
}
