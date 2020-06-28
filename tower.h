#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>


class QPainter;
class tScene;
class QTimer;
class tScene;
class Enemy;

class Tower : public QObject
{
    Q_OBJECT
public:
    Tower(QPoint pos, tScene *game, const QPixmap &sprite = QPixmap("://images/tower1.png"), int attackRange = 120, int damage = 10, int fireRate = 300, int level = 1);
    ~Tower();

    virtual void draw(QPainter *painter) const;
    void checkEnemyInRange();
    void targetKilled();
    void attackEnemy();
    void chooseEnemyForAttack(Enemy *enemy);
    void damageEnemy();
    void lostSightOfEnemy();

    //炮塔升级
    virtual void levelup() = 0;
    int             m_level;        // 等级
       const QPoint	m_pos; //塔的圆心
        QTimer *		m_fireRateTimer;

protected slots:
    //不同派生类（不同塔）需要发射不同子弹，需要虚函数
    virtual void shootWeapon() = 0;

protected:
    bool			m_attacking;
    qreal			m_rotationSprite;

    Enemy *			m_chooseEnemy;
    tScene *        m_game;


    int				m_attackRange;	// 代表塔可以攻击到敌人的距离
    int				m_damage;		// 代表攻击敌人时造成的伤害
    int				m_fireRate;		// 代表再次攻击敌人的时间间隔


    const QPixmap	m_sprite;

    static const QSize ms_fixedSize;
};

class Tower1: public Tower
{
    Q_OBJECT
public:
    Tower1(QPoint pos, tScene *game, const QPixmap &sprite = QPixmap("://images/tower1.png"));
    ~Tower1();

    void levelup();

protected slots:
    virtual void shootWeapon();
};

class Tower2 : public Tower
{
    Q_OBJECT
public:
    Tower2(QPoint pos, tScene *game, const QPixmap &sprite = QPixmap("://images/tower2.png"));
    ~Tower2();

    void levelup();

protected slots:
    virtual void shootWeapon();

private:
    int fireattack; //每秒的灼烧伤害
};



#endif // TOWER_H
