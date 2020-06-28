#ifndef CHOOSELEVEL_H
#define CHOOSELEVEL_H
#pragma once
#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <mainwindow.h>
#include <tScene.h>
#include <audioplayer.h>

namespace Ui {
class ChooseLevel;
}

class ChooseLevel : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseLevel(QWidget *parent = nullptr);
    ~ChooseLevel();
    void paintEvent(QPaintEvent *);
    AudioPlayer* audioPlayer;


private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ChooseLevel *ui;
};

#endif // CHOOSELEVEL_H
