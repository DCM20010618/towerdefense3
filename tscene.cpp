#include "tscene.h"
#include <QMediaPlayer>
#include <plistreader.h>
#include <waypoint.h>
#include <audioplayer.h>

static const int TowerCost = 300;

tScene::tScene(QWidget *parent) : QLabel(parent)
  , m_waves(0)
  , m_gameEnded(false)
  , m_gameWin(false)
  , m_playerHp(15)
  , m_playerGold(10000)
{
    this->setMouseTracking(true);
    this->grabKeyboard();
}

tScene::~tScene()
{
    if (!(this->MoneyFront == nullptr)) delete this->MoneyFront;
    if (!(this->MoneyBar == nullptr)) delete this->MoneyBar;
    if (!(this->moneybar == nullptr))delete this->moneybar;
    if (!(this->MoneyLabel == nullptr)) delete this->MoneyLabel;
    if (!(this->moneylabel == nullptr)) delete this->moneylabel;

    if (!(this->LifeFront == nullptr))delete this->LifeFront;
    if (!(this->LifeBar == nullptr)) delete this->LifeBar;
    if (!(this->lifebar == nullptr)) delete this->lifebar;
    if (!(this->LifeLabel == nullptr))delete this->LifeLabel;
    if (!(this->lifelabel == nullptr)) delete this->lifelabel;

    if (!(this->WaveFront == nullptr)) delete this->WaveFront;
    if (!(this->WaveBar == nullptr))delete this->WaveBar;
    if (!(this->wavebar == nullptr)) delete this->wavebar;
    if (!(this->WaveLabel == nullptr)) delete this->WaveLabel;
    if (!(this->wavelabel == nullptr))delete this->wavelabel;

}

void tScene::getHpDamage(int damage)
{
    //m_audioPlayer->playSound(LifeLoseSound);
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        doGameOver();
}


void tScene::removedBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void tScene::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void tScene::awardGold(int gold)
{
    m_playerGold += gold;
    update();
}

AudioPlayer *tScene::audioPlayer() const
{
    return m_audioPlayer;
}

void tScene::FireIceattack()
{
    foreach(Enemy *enemy, m_enemyList){
        if (enemy->fire != 0){
            enemy->fire--;
            enemy->getFireDamage(enemy->fireattack);
        }
        if(enemy->ice != 0){
            enemy->ice--;
            if (enemy->ice == 0)
            {
                enemy->m_walkingSpeed = enemy->m_normalSpeed;
                enemy->m_slowSpeed = enemy->m_normalSpeed;
            }
            else
            {
                enemy->m_walkingSpeed = enemy->m_slowSpeed;
            }
        }
    }
}

void tScene::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;

        m_audioPlayer->stopBGM();
        m_audioPlayer->playLoseSound();
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

QList<Enemy *> tScene::enemyList() const
{
    return m_enemyList;
}


Easy::Easy(QWidget* parent)
    : tScene(parent)
{
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/easymap.mp3");
    m_audioPlayer = new AudioPlayer(backgroundMusicUrl,this);
    m_audioPlayer->startBGM();

    this->setGeometry(0, 0, 1280, 720);
    this->setMovie(this->background);
    this->background->start();
    this->show();

    preLoadWavesInfo(); //设置波数
    loadTowerPositions(); //调用位置函数
    addWayPoints();

    //每100ms更新一次灼烧状态
    QTimer *Firetime = new QTimer(this);
    connect(Firetime, SIGNAL(timeout()), this, SLOT(FireIceattack()));
    Firetime->start(100);

    //每30ms发送一个更新信号
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    this->uiSetup();
    // 设置300ms后游戏启动
    QTimer::singleShot(300, this, SLOT(gameStart()));
}

void Easy::uiSetup()
{
    MoneyFront->setGeometry(42,660,200, 100);
    MoneyFront->setFont(QFont("Calibri", 20));
    MoneyFront->setText("50");
    MoneyFront->setAlignment(Qt::AlignHCenter);
    MoneyFront->show();
    MoneyFront->raise();
    MoneyLabel->setGeometry(16, 630,80, 80);
    moneylabel->start();
    MoneyLabel->show();
    MoneyLabel->setMovie(moneylabel);

    LifeFront->setGeometry(298, 660,200, 100);
    LifeFront->setFont(QFont("Calibri", 20));
    LifeFront->setText("10");
    LifeFront->setAlignment(Qt::AlignHCenter);
    LifeFront->show();
    LifeFront->raise();
    LifeLabel->setGeometry(300, 630, 80, 80);
    lifelabel->start();
    LifeLabel->show();
    LifeLabel->setMovie(lifelabel);

    WaveFront->setGeometry(564, 660, 200,100);
    WaveFront->setFont(QFont("Calibri", 20));
    WaveFront->setText("1");
    WaveFront->setAlignment(Qt::AlignHCenter);
    WaveFront->show();
    WaveFront->raise();
    WaveLabel->setGeometry(580, 630, 80, 80);
    wavelabel->start();
    WaveLabel->show();
    WaveLabel->setMovie(wavelabel);

    Base->setGeometry(1203, 219, 80, 80);
    base->start();
    Base->show();
    Base->setMovie(base);
    Base->raise();

    exit->setGeometry(700, 0, 60, 60); //设置退出按钮
    exit->setFlat(true);
    exit->setIcon(QIcon(":/image/Leave.png"));
    exit->setIconSize(QSize(60,60));
    exit->setStyleSheet("background: transparent");
    exit->setCursor(Qt::PointingHandCursor);
    connect(exit, SIGNAL(clicked()), this, SLOT(leave()));
    exit->show();
    exit->raise();
}

void Easy::removedEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty())
    {
        ++m_waves;
        if (!loadWave())
        {
        m_gameWin = true;
        m_audioPlayer->stopBGM();
        m_audioPlayer->playWinSound();
        // 游戏胜利转到游戏胜利场景
        // 这里暂时以打印处理
        }
    }
}


void Easy::loadTowerPositions()
{
    QPoint pos[] =
    {
        QPoint(81, 242),
        QPoint(236, 47),
        QPoint(336, 47),
        QPoint(436, 47),
        QPoint(536, 47),
        QPoint(304, 294),
        QPoint(404, 294),
        QPoint(504, 294),
        QPoint(860, 225),
        QPoint(760, 225),
        QPoint(647, 461),
        QPoint(747, 461),
        QPoint(847, 461),
        QPoint(947, 461),
        QPoint(1013, 86),
        QPoint(1115, 328)
    };
    int len	= sizeof(pos) / sizeof(pos[0]);

    for (int i = 0; i < len; ++i)
        m_towerPositionsList.push_back(pos[i]);
}


void Easy::addWayPoints()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(1239, 254));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(1044, 261));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(1031, 399));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(674,405 ));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(663,233 ));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(242,234 ));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);

        WayPoint *wayPoint7 = new WayPoint(QPoint(236,426));
        m_wayPointsList.push_back(wayPoint7);
        wayPoint7->setNextWayPoint(wayPoint6);

        WayPoint *wayPoint8 = new WayPoint(QPoint(40,423 ));
        m_wayPointsList.push_back(wayPoint8);
        wayPoint8->setNextWayPoint(wayPoint7);

}

bool Easy::loadWave()
{
    if (m_waves >= m_wavesInfo.size())
        return false;

    WayPoint *startWayPoint = m_wayPointsList.back();
    QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        Enemy *enemy;
        int j=i%3;
        switch(j){
        case 0:
            enemy = new Enemy1(startWayPoint, this);
            break;
        case 1:
            enemy=new Enemy2(startWayPoint, this);
            break;
        case 2:
            enemy=new Enemy3(startWayPoint, this);
            break;
        }
        m_enemyList.push_back(enemy);
        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
    }

    return true;
}

bool Easy::canBuyTower() const
{
    if (m_playerGold >= TowerCost)
        return true;
    return false;
}

void Easy::drawWave()
{
    WaveFront->setText(QString("%1").arg(m_waves +1));
    WaveFront->setAlignment(Qt::AlignHCenter);
    WaveFront->show();
    WaveFront->raise();
}

void Easy::drawHP()
{
    LifeFront->setText(QString("%1").arg(m_playerHp));
    LifeFront->setAlignment(Qt::AlignHCenter);
    LifeFront->show();
    LifeFront->raise();
}

void Easy::drawPlayerGold()
{
    MoneyFront->setText(QString("%1").arg(m_playerGold));
    MoneyFront->setAlignment(Qt::AlignHCenter);
    MoneyFront->show();
    MoneyFront->raise();
}


void Easy::preLoadWavesInfo()
{
    QFile file(":/config/Waves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    // 获取波数信息
    m_wavesInfo = reader.data();

    file.close();
}

Easy::~Easy()
{
    delete this->background;
    delete this->exit;

    foreach (Tower *tower, m_towersList)
    {
        Q_ASSERT(tower);
        m_towersList.removeOne(tower);
        delete tower;
    }
    foreach (Enemy *enemy, m_enemyList)
    {
        Q_ASSERT(enemy);
        m_enemyList.removeOne(enemy);
        delete enemy;
    }
    foreach (Bullet *bullet, m_bulletList)
    {
        removedBullet(bullet);
    }
    delete Front1;
    delete Front2;
    delete Front3;
    delete Front4;

    //delete ui;
}

void Easy::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos();
    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
    {
        if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
        {

            m_playerGold -= TowerCost;
            it->setHasTower();

            Tower *tower = new Tower1(it->centerPos(), this);
            m_towersList.push_back(tower);
            audioPlayer()->playSound(TowerPlaceSound);
            update();
            break;
        }
        ++it;
    }

}


void Easy::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        emit toTitle();
        break;
    }
}

void Easy::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)
    {
        MoneyFront->hide();
        MoneyBar->hide();
        MoneyLabel->hide();
        LifeFront->hide();
        LifeBar->hide();
        LifeLabel->hide();
        WaveFront->hide();
        WaveBar->hide();
        WaveLabel->hide();
        Base->hide();
        Front1->hide();
        Front2->hide();
        Front3->hide();
        Front4->hide();

        foreach (Tower *tower, m_towersList)
        {
            Q_ASSERT(tower);
            m_towersList.removeOne(tower);
            delete tower;
        }
        foreach (Enemy *enemy, m_enemyList)
        {
            Q_ASSERT(enemy);
            m_enemyList.removeOne(enemy);
            delete enemy;
        }
        foreach (Bullet *bullet, m_bulletList)
        {
            removedBullet(bullet);
        }

        if(m_gameWin){
            QString text =  "YOU WIN!!!";
            QPainter painter(this);
            painter.setPen(QPen(Qt::red));
            painter.drawText(rect(), Qt::AlignCenter, text);
            audioPlayer()->playWinSound();

        }

        if(m_gameEnded)
        {
            QString text =  "YOU LOST!!!" ;
            QPainter painter(this);
            painter.setPen(QPen(Qt::red));
            painter.drawText(rect(), Qt::AlignCenter, text);
            audioPlayer()->playLoseSound();
        }
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawPixmap(0,0,1280,720, QPixmap("://images/easymap.jpg"));
    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&painter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&painter);

    //foreach (const WayPoint *wayPoint, m_wayPointsList)
    //    wayPoint->draw(&painter);

    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&painter);

    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&painter);
    drawWave();
    drawHP();
    drawPlayerGold();
}

void Easy::onTimer()
{
    this->exit->raise();
}

void Easy::leave()
{
    emit toTitle();
}

void Easy::back()
{

}

void Easy::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    update();
}

void Easy::gameStart()
{
    loadWave();
}

// 以下是对复杂地图的设计

Hard::Hard(QWidget* parent)
    : tScene(parent)
{
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/Hardmap.mp3");
    m_audioPlayer = new AudioPlayer(backgroundMusicUrl,this);
    m_audioPlayer->startBGM();

    this->setGeometry(0, 0, 1280, 720);
    this->setMovie(this->background);
    this->background->start();
    this->show();

    preLoadWavesInfo(); //设置波数
    loadTowerPositions(); //调用位置函数
    addWayPoints();

    //每100ms更新一次灼烧状态
    QTimer *Firetime = new QTimer(this);
    connect(Firetime, SIGNAL(timeout()), this, SLOT(FireIceattack()));
    Firetime->start(100);

    //每30ms发送一个更新信号
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    this->uiSetup();
    // 设置300ms后游戏启动
    QTimer::singleShot(300, this, SLOT(gameStart()));
}

void Hard::uiSetup()
{
    MoneyFront->setGeometry(42,660,200, 100);
    MoneyFront->setFont(QFont("Calibri", 20));
    MoneyFront->setText("50");
    MoneyFront->setAlignment(Qt::AlignHCenter);
    MoneyFront->show();
    MoneyFront->raise();
    MoneyLabel->setGeometry(16, 630,80, 80);
    moneylabel->start();
    MoneyLabel->show();
    MoneyLabel->setMovie(moneylabel);

    LifeFront->setGeometry(298, 660,200, 100);
    LifeFront->setFont(QFont("Calibri", 20));
    LifeFront->setText("10");
    LifeFront->setAlignment(Qt::AlignHCenter);
    LifeFront->show();
    LifeFront->raise();
    LifeLabel->setGeometry(300, 630, 80, 80);
    lifelabel->start();
    LifeLabel->show();
    LifeLabel->setMovie(lifelabel);

    WaveFront->setGeometry(564, 660, 200,100);
    WaveFront->setFont(QFont("Calibri", 20));
    WaveFront->setText("1");
    WaveFront->setAlignment(Qt::AlignHCenter);
    WaveFront->show();
    WaveFront->raise();
    WaveLabel->setGeometry(580, 630, 80, 80);
    wavelabel->start();
    WaveLabel->show();
    WaveLabel->setMovie(wavelabel);

    Base->setGeometry(1203, 265, 80, 80);
    base->start();
    Base->show();
    Base->setMovie(base);
    Base->raise();

    exit->setGeometry(700, 0, 60, 60); //设置退出按钮
    exit->setFlat(true);
    exit->setIcon(QIcon(":/image/Leave.png"));
    exit->setIconSize(QSize(60,60));
    exit->setStyleSheet("background: transparent");
    exit->setCursor(Qt::PointingHandCursor);
    connect(exit, SIGNAL(clicked()), this, SLOT(leave()));
    exit->show();
    exit->raise();
}

void Hard::removedEnemy(Enemy *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty())
    {
        ++m_waves;
        if (!loadWave())
        {
        m_gameWin = true;
        m_audioPlayer->stopBGM();
        m_audioPlayer->playWinSound();
        // 游戏胜利转到游戏胜利场景
        // 这里暂时以打印处理
        }
    }
}


void Hard::loadTowerPositions()
{
    QPoint pos[] =
    {
        QPoint(245,18 ),
        QPoint(345,18 ),
        QPoint(445,18 ),
        QPoint(147,230 ),
        QPoint(247,230 ),
        QPoint(347,230 ),
        QPoint(447,230 ),
        QPoint(447,330 ),
        QPoint(447,430 ),
        QPoint(721,297 ),
        QPoint(821,297 ),
        QPoint(772,532 ),
        QPoint(872,532 ),
        QPoint(953,146 ),
        QPoint(1053,146 ),
        QPoint(1153,146 ),
        QPoint(1020, 363)
    };
    int len	= sizeof(pos) / sizeof(pos[0]);

    for (int i = 0; i < len; ++i)
        m_towerPositionsList.push_back(pos[i]);
}


void Hard::addWayPoints()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(1244, 313));
        m_wayPointsList.push_back(wayPoint1);

        WayPoint *wayPoint2 = new WayPoint(QPoint(953, 313));
        m_wayPointsList.push_back(wayPoint2);
        wayPoint2->setNextWayPoint(wayPoint1);

        WayPoint *wayPoint3 = new WayPoint(QPoint(953, 445));
        m_wayPointsList.push_back(wayPoint3);
        wayPoint3->setNextWayPoint(wayPoint2);

        WayPoint *wayPoint4 = new WayPoint(QPoint(612,445 ));
        m_wayPointsList.push_back(wayPoint4);
        wayPoint4->setNextWayPoint(wayPoint3);

        WayPoint *wayPoint5 = new WayPoint(QPoint(612,176 ));
        m_wayPointsList.push_back(wayPoint5);
        wayPoint5->setNextWayPoint(wayPoint4);

        WayPoint *wayPoint6 = new WayPoint(QPoint(97,176 ));
        m_wayPointsList.push_back(wayPoint6);
        wayPoint6->setNextWayPoint(wayPoint5);


}

bool Hard::loadWave()
{
    if (m_waves >= m_wavesInfo.size())
        return false;

    WayPoint *startWayPoint = m_wayPointsList.back();
    QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

    for (int i = 0; i < curWavesInfo.size(); ++i)
    {
        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
        int spawnTime = dict.value("spawnTime").toInt();

        Enemy *enemy;
        int j=i%3;
        switch(j){
        case 0:
            enemy = new Enemy1(startWayPoint, this);
            break;
        case 1:
            enemy=new Enemy2(startWayPoint, this);
            break;
        case 2:
            enemy=new Enemy3(startWayPoint, this);
            break;
        }
        m_enemyList.push_back(enemy);
        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
    }

    return true;
}

bool Hard::canBuyTower() const
{
    if (m_playerGold >= TowerCost)
        return true;
    return false;
}

void Hard::drawWave()
{
    WaveFront->setText(QString("%1").arg(m_waves +1));
    WaveFront->setAlignment(Qt::AlignHCenter);
    WaveFront->show();
    WaveFront->raise();
}

void Hard::drawHP()
{
    LifeFront->setText(QString("%1").arg(m_playerHp));
    LifeFront->setAlignment(Qt::AlignHCenter);
    LifeFront->show();
    LifeFront->raise();
}

void Hard::drawPlayerGold()
{
    MoneyFront->setText(QString("%1").arg(m_playerGold));
    MoneyFront->setAlignment(Qt::AlignHCenter);
    MoneyFront->show();
    MoneyFront->raise();
}


void Hard::preLoadWavesInfo()
{
    QFile file(":/config/hardWaves.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    // 获取波数信息
    m_wavesInfo = reader.data();

    file.close();
}

Hard::~Hard()
{
    delete this->background;
    delete this->exit;

    foreach (Tower *tower, m_towersList)
    {
        Q_ASSERT(tower);
        m_towersList.removeOne(tower);
        delete tower;
    }
    foreach (Enemy *enemy, m_enemyList)
    {
        Q_ASSERT(enemy);
        m_enemyList.removeOne(enemy);
        delete enemy;
    }
    foreach (Bullet *bullet, m_bulletList)
    {
        removedBullet(bullet);
    }
    delete Front1;
    delete Front2;
    delete Front3;
    delete Front4;

    //delete ui;
}


void Hard::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        emit toTitle();
        break;
    }
}

void Hard::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)
    {
        MoneyFront->hide();
        MoneyBar->hide();
        MoneyLabel->hide();
        LifeFront->hide();
        LifeBar->hide();
        LifeLabel->hide();
        WaveFront->hide();
        WaveBar->hide();
        WaveLabel->hide();
        Base->hide();
        Front1->hide();
        Front2->hide();
        Front3->hide();
        Front4->hide();

        foreach (Tower *tower, m_towersList)
        {
            Q_ASSERT(tower);
            m_towersList.removeOne(tower);
            delete tower;
        }
        foreach (Enemy *enemy, m_enemyList)
        {
            Q_ASSERT(enemy);
            m_enemyList.removeOne(enemy);
            delete enemy;
        }
        foreach (Bullet *bullet, m_bulletList)
        {
            removedBullet(bullet);
        }

        if(m_gameWin){
            QString text =  "YOU WIN!!!";
            QPainter painter(this);
            painter.setPen(QPen(Qt::red));
            painter.drawText(rect(), Qt::AlignCenter, text);
            audioPlayer()->playWinSound();

        }

        if(m_gameEnded)
        {
            QString text =  "YOU LOST!!!" ;
            QPainter painter(this);
            painter.setPen(QPen(Qt::red));
            painter.drawText(rect(), Qt::AlignCenter, text);
            audioPlayer()->playLoseSound();
        }
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawPixmap(0,0,1280,720, QPixmap("://images/hardmap.jpg"));
    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw2(&painter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&painter);

    //foreach (const WayPoint *wayPoint, m_wayPointsList)
    //    wayPoint->draw(&painter);

    foreach (const Enemy *enemy, m_enemyList)
        enemy->draw(&painter);

    foreach (const Bullet *bullet, m_bulletList)
        bullet->draw(&painter);
    drawWave();
    drawHP();
    drawPlayerGold();
}

void Hard::onTimer()
{
    this->exit->raise();
}

void Hard::leave()
{
    emit toTitle();
}

void Hard::back()
{

}

void Hard::updateMap()
{
    foreach (Enemy *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    update();
}

void Hard::gameStart()
{
    loadWave();
}

void Hard::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pressPos=event->pos();
    auto it=m_towerPositionsList.begin();
    while(it!=m_towerPositionsList.end())
    {
        if(it->containPoint(pressPos)&&it->hasTower())
        {
            it->setHasTower(false);
            int i;
            for(i=0;i<m_towersList.size();i++)
            {
                if(it->containPoint(m_towersList[i]->m_pos))
                {
                    m_towersList[i]->m_fireRateTimer->stop();
                    m_playerGold+=m_towersList[i]->m_level*100;
                    m_towersList.erase(m_towersList.begin()+i);
                    audioPlayer()->playSound(BreakTowersound);
                    it->setHasTower(false);
                }
            }
            update();
            break;
        }
        ++it;
    }
}

void Hard::mousePressEvent(QMouseEvent *event)
{
    // 如果是鼠标左键按下
    if(event->button() == Qt::LeftButton)
   {
        QPoint pressPos = event->pos();
        auto it = m_towerPositionsList.begin();
        while (it != m_towerPositionsList.end())
        {
            if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
            {

                m_playerGold -= TowerCost;
                it->setHasTower();

                Tower *tower = new Tower1(it->centerPos(), this);
                m_towersList.push_back(tower);
                audioPlayer()->playSound(TowerPlaceSound);
                update();
                break;
            }

            ++it;
        }
    }
    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
   {
        QPoint pressPos = event->pos();
        auto it = m_towerPositionsList.begin();
        while (it != m_towerPositionsList.end())
        {
            if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
            {

                m_playerGold -= TowerCost;
                it->setHasTower();

                Tower *tower = new Tower2(it->centerPos(), this);
                m_towersList.push_back(tower);
                audioPlayer()->playSound(TowerPlaceSound);
                update();
                break;
            }

            ++it;
        }
    }

}
