#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,720);
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/mainwindow.mp3");
    audioPlayer = new AudioPlayer(backgroundMusicUrl,this);
    audioPlayer->startBGM();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, 1280,720,QPixmap("://images/menu.png"));

}

void MainWindow::on_pushButton_clicked()
{
    ChooseLevel* d=new ChooseLevel();
    audioPlayer->stopBGM();
    this->hide();
    d->show();

}
