#include "chooselevel.h"
#include "ui_chooselevel.h"
#include <QPainter>
#include <QPaintEvent>


ChooseLevel::ChooseLevel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseLevel)
{
    ui->setupUi(this);
    setFixedSize(1280,720);
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/mainwindow.mp3");
    audioPlayer = new AudioPlayer(backgroundMusicUrl,this);
    audioPlayer->startBGM();
}

ChooseLevel::~ChooseLevel()
{
    delete ui;
}

void ChooseLevel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, 1280,720,QPixmap("://images/chooselevelscene.png"));

}

void ChooseLevel::on_pushButton_3_clicked()
{
    MainWindow *d=new MainWindow;
    this->hide();
    audioPlayer->stopBGM();
    d->show();
}

void ChooseLevel::on_pushButton_2_clicked()
{
    Easy *d=new Easy;
    this->hide();
    audioPlayer->stopBGM();
    d->show();
}

void ChooseLevel::on_pushButton_clicked()
{
    Hard *d=new Hard;
    this->hide();
    audioPlayer->stopBGM();
    d->show();
}
