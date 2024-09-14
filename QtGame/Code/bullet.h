#ifndef BULLET_H
#define BULLET_H
#include<QString>
#include"enemyunit.h"
class EnemyUnit;
class Bullet
{
    double b_x;
    double b_y;
    int pic_width;
    int pic_height;
    EnemyUnit* target;
    qreal k;
    int speed;
    int type;
public:
    Bullet(double,double,int,int);
    void move();
    void settarget(EnemyUnit* eu);
    QString rpic();
    bool reachtarget();
    double get_x();
    double get_y();
    int ret_picw ();
    int ret_pich();
    EnemyUnit* ret_tg();
    qreal cal_angle();
};

#endif // BULLET_H
