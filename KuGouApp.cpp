#include "KuGouApp.h"
#include "ui_KuGouApp.h"
#include"TableWidget.h"
#include"MusicItemWidget.h"

#include<QMediaPlayer>
#include<QAudioOutput>
#include<QFileDialog>
#include<QStandardPaths>
#include<QDir>
KuGouApp::KuGouApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KuGouApp)
    ,m_player(std::make_unique<QMediaPlayer>())
    ,m_audioOutput(std::make_unique<QAudioOutput>())
{
    ui->setupUi(this);
    initPoster();
    initTabWidget();
}

KuGouApp::~KuGouApp()
{
    delete ui;
}

void KuGouApp::initPoster()
{
    QDir dir(__FILE__);
    if(dir.cdUp())dir.cd("image/poster");

    auto s = dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    for(auto i = 1 ; i <= s ; ++i)
        ui->advertise_board_widget->addPoster(QPixmap(QString("://image/poster/%1.jpg").arg(i)));
}

void KuGouApp::initTabWidget()
{
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->table_widget->layout());
    if(!layout)return;
    layout->insertWidget(layout->count() -1 ,new TableWidget("潮流音乐站",this));
    layout->insertWidget(layout->count() -1 ,new TableWidget("热门好歌精选",this));
    layout->insertWidget(layout->count() -1 ,new TableWidget("私人专属好歌",this));

}

void KuGouApp::on_basic_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->main_page);
}


void KuGouApp::on_local_download_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->local_download_page);
}


void KuGouApp::on_local_add_toolButton_clicked() // 此处后面需要优化 ， 即使批量添加也没问题才行
{
    QString musicPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
    QString path = QFileDialog::getOpenFileName(this,"添加音乐",musicPath,"Music (*.mp3 *.aac *.wav)");
    if(path.isEmpty()){
        return;
    }
    this->m_locationMusicMap[this->m_locationMusicMap.size() + 1] = QUrl::fromLocalFile(path);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(ui->local_song_list_widget->layout());
    if(!layout)return;
    int index = this->m_locationMusicMap.size();
    QString fileName = this->m_locationMusicMap[index].fileName();
    auto item = new MusicItemWidget(index,fileName.remove(fileName.lastIndexOf("."),fileName.length()),this);
    connect(item,&MusicItemWidget::playRequest,this,[this](int index){
        if(this->m_isPlaying)ui->play_or_pause_toolButton->click();
        setPLayMusic(this->m_locationMusicMap[index]);
    });
    layout->insertWidget(layout->count() - 1,item);
    ui->local_number__label->setText(QString::number(this->m_locationMusicMap.size()));
}


void KuGouApp::on_play_or_pause_toolButton_clicked()
{
    if(this->m_isPlaying){

    }
}

void KuGouApp::setPLayMusic(const QUrl &url)
{

}

