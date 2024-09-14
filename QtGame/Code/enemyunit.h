#ifndef ENEMYUNIT_H
#define ENEMYUNIT_H
#include"unit.h"
#include"grid.h"
#include<QVector>
#include"myunit.h"
//用路径入口区分飞行路径和地面路径
//用int型区分不同的地面单位和飞行单位
//grd_type:
//0:近程攻击
//1:远程攻击
//fly_type:
//0:恋战的
//1:畏战的
//etype:
//0:地面单位
//1:飞行单位
//2:能给己方加血
class MyUnit;
class EnemyUnit:public Unit
{

protected:
    QVector<Grid*> e_path;//每一个敌人都有一个自己的路径
    int speed;
    int e_type;
    MyUnit* target;
    int state;
public:
    EnemyUnit();
    EnemyUnit(QVector<int*> &p);
    void move();
    virtual QString Enemypic()=0;
    virtual int get_type()=0;
    int get_etype();
    double distance(MyUnit* m);//计算距离
    virtual bool meet_myunit(MyUnit* m)=0;
    virtual void set_target(MyUnit*)=0;
   /* virtual */void attack();
    virtual void get_hurt(int hurt);
    MyUnit* get_target();
    void set_null();
    bool at_end();
    virtual ~EnemyUnit();
    void blood_increase(int);
    virtual void add_blood(EnemyUnit*);
};
class grd_enemyunit:public EnemyUnit
{
    int grd_type;
public:
    grd_enemyunit(QVector<int*>&p,int type);
    int get_type() ;
    QString Enemypic() ;
    bool meet_myunit(MyUnit *m);
    void set_target(MyUnit *);
};
class fly_enemyunit:public EnemyUnit
{
    int fly_type;
public:
    fly_enemyunit(QVector<int*>& p,int type);
    int get_type() ;
    QString Enemypic() ;
    void set_target(MyUnit *);
    bool meet_myunit(MyUnit *m);
};
class assist_enemy:public EnemyUnit{
        EnemyUnit* ta=NULL;
public:
    assist_enemy(QVector<int*>& p);
    int get_type();
    QString Enemypic();
    void set_target(MyUnit *);
    bool meet_myunit(MyUnit *m);
    void add_blood(EnemyUnit*);
    double com_dis(EnemyUnit*);
};

#endif // ENEMYUNIT_H
