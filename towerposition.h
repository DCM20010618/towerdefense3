#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <tower.h>

class QPainter;

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap(":/images/towerbase1.png"));

    void setHasTower(bool hasTower = true);
    bool hasTower() const;
    const QPoint centerPos() const;
    bool containPoint(const QPoint &pos) const;

    void draw(QPainter *painter) const;
    void draw2(QPainter *painter) const;
    int         m_towerkind;
    Tower*       m_tower;
private:
    bool		m_hasTower;
    QPoint		m_pos;
    QPixmap		m_sprite;


    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
