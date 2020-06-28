#ifndef TSCENE_H
#define TSCENE_H

#include <QDialog>
#include <QMovie>
#include <QList>
#include <QLabel>
#include "towerposition.h"
#include "tower.h"
#include <QMovie>
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QMediaPlayer>
#include <audioplayer.h>
#include "bullet.h"
#include "enemy.h"

class WayPoint;
class Enemy;
class Bullet;
class AudioPlayer;
class tCard;
class AudioPlayer;


class Bullet;
class tCard;
class TowerPosition;

class tScene : public QLabel
{
    Q_OBJECT
public:
    int sunPoint = 50;
    tCard* currentCard = nullptr;
    QPoint currentPos;
    QList<tCard*> Cards;
    QPoint m;
    int currentIndex = -1;
    int upgradestate = 0;
    Tower* currenttower;

    explicit tScene(QWidget *parent = 0);
    ~tScene();

    QTimer* timer = nullptr;

    QLabel* MoneyFront = new QLabel(this);
    QLabel* MoneyBar = new QLabel(this);
    QMovie* moneybar = new QMovie("://images/info_bar.png");
    QLabel* MoneyLabel = new QLabel(this);
    QMovie* moneylabel = new QMovie("://images/money.png");

    QLabel* LifeFront = new QLabel(this);
    QLabel* LifeBar = new QLabel(this);
    QMovie* lifebar = new QMovie("://images/info_bar.png");
    QLabel* LifeLabel = new QLabel(this);
    QMovie* lifelabel = new QMovie("://images/life.png");

    QLabel* WaveFront = new QLabel(this);
    QLabel* WaveBar = new QLabel(this);
    QMovie* wavebar = new QMovie("://images/info_bar.png");
    QLabel* WaveLabel = new QLabel(this);
    QMovie* wavelabel = new QMovie("://images/wave.png");

    QLabel* Front1 = new QLabel(this);
    QLabel* Front2 = new QLabel(this);
    QLabel* Front3 = new QLabel(this);
    QLabel* Front4 = new QLabel(this);


    QLabel* Base = new QLabel(this);
    QMovie* base = new QMovie("://images/dangao.png");


    // move to base class
    void addBullet(Bullet *bullet);
    virtual void removedEnemy(Enemy *enemy) = 0;
    void removedBullet(Bullet *bullet);
    QList<Enemy *> enemyList() const;
    void awardGold(int gold);
    void getHpDamage(int damage);
    void doGameOver();
    AudioPlayer* audioPlayer() const;

    QList<Bullet *>			m_bulletList;
    QList<Enemy *>			m_enemyList;
    QList<Tower *>			m_towersList;
    int						m_waves;
    bool					m_gameEnded;
    bool					m_gameWin;
    int						m_playerHp;
    int						m_playerGold;
    AudioPlayer *		    m_audioPlayer;

signals:
    void toTitle(); //返回信号，返回主界面
    void totScene();
    void toHard();

public slots:
    void FireIceattack();
};


class Easy : public tScene
{
    Q_OBJECT
public:
    explicit Easy(QWidget* parent = 0);
    ~Easy();
    virtual void removedEnemy(Enemy *enemy);

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);

private:
    QMovie* background = new QMovie("://images/easymap.jpg");
    QPushButton* exit = new QPushButton(this);

    QMovie* station = new QMovie("//images/towerbase1.png");

    //QPoint cell;
    void uiSetup();

    //增加代码 6-6
private:
    void loadTowerPositions();
    void addWayPoints();
    bool loadWave();
    bool canBuyTower() const;
    void drawWave();
    void drawHP();
    void drawPlayerGold();
    //void doGameOver();
    void preLoadWavesInfo();

private:
    QList<QVariant>			m_wavesInfo;
    QList<TowerPosition>	m_towerPositionsList; //√
    QList<WayPoint *>		m_wayPointsList;

private slots:
    void onTimer();
    void leave(); // emit toTitle();

    void back();

    void updateMap(); //原来的槽
    void gameStart(); //原来的槽

};


class Hard : public tScene
{
    Q_OBJECT
public:
    explicit Hard(QWidget* parent = 0);
    ~Hard();
    virtual void removedEnemy(Enemy *enemy);




protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *);

private:
    QMovie* background = new QMovie("://images/hardmap.jpg");
    QPushButton* exit = new QPushButton(this);

    QMovie* station = new QMovie("://images/towerbase2.png");
    //QPoint cell;
    void uiSetup();

private:
    void loadTowerPositions(); //√
    //for different enemies
    void addWayPoints();


    bool loadWave();
    bool canBuyTower() const; //√
    void drawWave();
    void drawHP();
    void drawPlayerGold();
    void preLoadWavesInfo();

private:
    QList<QVariant>			m_wavesInfo;
    QList<TowerPosition>	m_towerPositionsList; //√
     QList<WayPoint *>		m_wayPointsList;

private slots:
    void onTimer();
    void leave(); // emit toTitle();
    void back();

    void updateMap(); //原来的槽
    void gameStart(); //原来的槽
};


#endif // TSCENE_H
