#ifndef MYUNIT_H
#define MYUNIT_H
#include"unit.h"
#include<QVector>
#include"grid.h"
#include"enemyunit.h"
#include"bullet.h"
#include<QTimer>
#include<QMainWindow>
//mu_type:
//0:近战单位
//1:远战单位
//3:阻拦单位
//4:阻拦数为0的单位
//cls_type:
//0:只攻击地面单位
//1:能攻击飞行单位
//far_type:
//0:只攻击地面单位
//1:能攻击飞行单位
class Bullet;
class EnemyUnit;
class MyUnit:public Unit
{
protected:
      EnemyUnit* target;
      int mu_type;
      Bullet* bull;
      int i=0;

public:
    void get_hurt(int hurt);
    MyUnit();
    MyUnit(int,int,int type=0);
    virtual void attack_tg();
    void insert_tg(EnemyUnit*);
    virtual QString getmy_unpic()=0;
    int get_mutype();
    virtual int spec_type()=0;
    bool& whether_fight();
    void erase_en(EnemyUnit*);
    virtual bool meet_enemy(EnemyUnit*)=0;
    Bullet* ret_bull();
    virtual ~MyUnit();
    int ret_power();
    int ret_intib();
    int ret_nowb();
};
class cls_myunit:public MyUnit{
    int cls_type;
public:
    cls_myunit(int x,int y,int cls_type);
    bool meet_enemy(EnemyUnit *);
    QString getmy_unpic();
    int spec_type() ;
};
class far_myunit:public MyUnit{
    int far_type;
public:
    far_myunit(int x,int y,int far_type);
    bool meet_enemy(EnemyUnit *);
    QString getmy_unpic();
    void attack_tg();
    int spec_type();
};
class nut:public MyUnit
{
public:
    nut(int x,int y,int type);
    QString getmy_unpic();
    int spec_type();
    bool meet_enemy(EnemyUnit*);
};
class spikeweed:public MyUnit
{
    QTimer* bloodtimer;
public:
    spikeweed(int x,int y,int type,QMainWindow*);
    QString getmy_unpic();
    int spec_type();
    bool meet_enemy(EnemyUnit *);
};

#endif // MYUNIT_H
