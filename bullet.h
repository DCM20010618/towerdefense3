#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>


class QPainter;
class Enemy;
class Enemy1;
class Enemy2;
class MainWindow;
class tScene;


class Bullet : public QObject
{
    Q_OBJECT
    //Qt动画效果
    Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)

public:
    Bullet(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, tScene *game, int kind = 0, int fire = 0, qreal slow = 1, const QPixmap &sprite = QPixmap("://images/bullet1.png"));

    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;
    friend class Enemy;

protected slots:
    void hitTarget();

protected:
    const QPoint	m_startPos;
    const QPoint	m_targetPos;
    const QPixmap	m_sprite;
    QPoint			m_currentPos;
    Enemy *			m_target;
    tScene *        m_game;
    int				m_damage;
    int             bulletKind;
    int             fire_attack;
    qreal           slow_speed;

    static const QSize ms_fixedSize;
};

class Bullet1: public Bullet
{
    Q_OBJECT

public:
    Bullet1(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, tScene *game, const QPixmap &sprite = QPixmap("://images/bullet1.png"));
};

class Bullet2: public Bullet
{
    Q_OBJECT

public:
    Bullet2(QPoint startPos, QPoint targetPoint, int damage, Enemy *target, tScene *game, int kind = 1, int fire = 1, const QPixmap &sprite = QPixmap("://images/bullet2.png"));

};

#endif // BULLET_H
