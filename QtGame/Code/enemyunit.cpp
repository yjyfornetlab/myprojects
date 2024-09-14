#include "enemyunit.h"
#include<QTimer>
#include<QVector>
#include<QDebug>
#include"grid.h"
#include"numdefine.h"
#include"myunit.h"
#include<QtMath>
#include"unit.h"
EnemyUnit::EnemyUnit()
{

}
EnemyUnit::EnemyUnit(QVector<int*> &path)
{
     target=NULL;
     for(auto p1:path)
     {
          e_path.push_back(new Grid(p1[1]*gsize,p1[0]*gsize));
     }
     u_x=(*e_path.begin())->returnx();
     u_y=(*e_path.begin())->returny();
     state=1;
}
void EnemyUnit::move()
{
     if(!isfighting)
     {
         if(e_path.empty())
            return;
         if(u_x<e_path.at(0)->returnx())
         {
             u_x+=speed;
             state=2;
         }
         else if(u_x>e_path.at(0)->returnx())
         {
             u_x-=speed;
             state=1;
         }
         if(u_y<e_path.at(0)->returny())
         {
             u_y+=speed;
             state=3;
         }
         else if(u_y>e_path.at(0)->returny())
         {
             u_y-=speed;
             state=4;
         }
         if(u_x==e_path.at(0)->returnx()&&u_y==e_path.at(0)->returny())
         {
             delete (*e_path.begin());
             e_path.erase(e_path.begin());
         }
     }
}
double EnemyUnit::distance(MyUnit* m)
{
     int dx=(get_x()-m->get_x());
     int dy=(get_y()-m->get_y());
     return qSqrt(dx*dx+dy*dy);
}
int EnemyUnit::get_etype()
{
    return e_type;
}

void EnemyUnit::attack()
{
    if(target==NULL)
    {
        isfighting=false;
        return;
    }
    else if(target!=NULL&&target->get_nb()<=0)
    {
        isfighting=false;
        target=NULL;
    }
    else
        target->get_hurt(att_power);
}
void EnemyUnit::get_hurt(int hurt)
{
    now_blood-=hurt;
}
MyUnit* EnemyUnit::get_target()
{
    return target;
}
void EnemyUnit::set_null()
{
    target=NULL;
}
bool EnemyUnit::at_end()
{
    if(e_path.empty())
        return true;
    else
        return false;
}
void EnemyUnit::blood_increase(int i)
{
    if(this->now_blood+i<=init_blood)
        now_blood+=i;
    else
        now_blood=init_blood;
}
void EnemyUnit::add_blood(EnemyUnit *){
    return;
}
EnemyUnit::~EnemyUnit()
{

}

//地面单位
grd_enemyunit::grd_enemyunit(QVector<int*>& p,int type):EnemyUnit(p)
{
    e_type=0;
    pic_height=gsize;
    pic_width=gsize*0.6;
    grd_type=type;
    speed=5;
    init_blood=100;
    now_blood=100;
    att_power=10;
    if(grd_type==0)
        att_range=gsize;
    else if(grd_type==1)
        att_range=2*gsize;
}
int grd_enemyunit::get_type()
{
    return grd_type;
}
QString grd_enemyunit::Enemypic()
{

    if(grd_type==0)
    {
        if(state==1)
            return ":/tupian/image0_1.png";
        else if(state==2)
            return ":/tupian/image0_2.png";
        else if(state==3)
            return ":/tupian/image0_3.png";
        else
            return ":/tupian/image0_4.png";
    }
    else if(grd_type==1)
    {
        if(state==1)
            return ":/tupian/image1_1.png";
        else if(state==2)
            return ":/tupian/image1_2.png";
        else if(state==3)
            return ":/tupian/image1_3.png";
        else
            return ":/tupian/image1_4.png";
    }

}
bool grd_enemyunit::meet_myunit(MyUnit *m)
{
    if(m->get_mutype()==0||m->get_mutype()==3)
    {
        if(distance(m)<=att_range)
            return true;
        else
            return false;
    }
    else
        return false;
}
void grd_enemyunit::set_target(MyUnit* mu)
{
   isfighting=true;
   target=mu;
}

//飞行单位
fly_enemyunit::fly_enemyunit(QVector<int*>& p,int type):EnemyUnit(p)
{
    e_type=1;
    fly_type=type;
    if(fly_type==0)
    {
        pic_height=gsize;
        pic_width=gsize*0.5;
    }
    else
    {
        pic_height=gsize;
        pic_width=gsize;
    }
    speed=5;
    init_blood=200;
    now_blood=200;
    att_range=2.5*gsize;
    att_power=10;
}
int fly_enemyunit::get_type()
{
    return fly_type;
}
bool fly_enemyunit::meet_myunit(MyUnit *m)
{
    if(distance(m)<=att_range)
        return true;
    else
        return false;
}
void fly_enemyunit::set_target(MyUnit * mu)
{
    if(fly_type==0)
        isfighting=true;
    target=mu;
}
//state:
//1: 向左
//2: 向右
//3: 向下
//4: 向上
QString fly_enemyunit::Enemypic()
{
    if(fly_type==0)
    {
        if(this->state==1)
            return ":/tupian/flyenemy1_1.png";
        else if(this->state==2)
            return ":/tupian/flyenemy1_2.png";
        else if(this->state==3)
            return ":/tupian/flyenemy1_3.png";
        else
            return ":/tupian/flyenemy1_4.png";
    }
    else if(fly_type==1)
    {
        if(this->state==1)
            return ":/tupian/flyenemy2_1.png";
        else if(this->state==2)
            return ":/tupian/flyenemy2_2.png";
        else if(this->state==3)
            return ":/tupian/flyenemy2_3.png";
        else
            return ":/tupian/flyenemy2_4.png";
    }
}

//辅助加血单位
assist_enemy::assist_enemy(QVector<int*>& p):EnemyUnit(p)
{
    att_range=gsize;
    e_type=2;
    pic_height=gsize;
    pic_width=gsize*0.6;
    speed=2;
    init_blood=100;
    now_blood=100;
    att_power=2;
}
int assist_enemy::get_type()
{
    return 0;
}
QString assist_enemy::Enemypic()
{
    if(state==1)
        return ":/tupian/assist_1.png";
    else if(state==2)
        return ":/tupian/assist_2.png";
    else if(state==3)
        return ":/tupian/assist_3.png";
    else
        return ":/tupian/assist_4.png";
}
void assist_enemy::set_target(MyUnit *)
{
    return;
}
bool assist_enemy::meet_myunit(MyUnit *m)
{
    return false;
}
double assist_enemy::com_dis(EnemyUnit * en)
{
    int dx=(get_x()-en->get_x());
    int dy=(get_y()-en->get_y());
    return qSqrt(dx*dx+dy*dy);
}
void assist_enemy::add_blood(EnemyUnit* en){
    if(en->get_etype()==0&&ta==NULL)
    {
        if(this->com_dis(en)<=att_range&&en->get_nb()<en->get_ib())
        {
            ta=en;
            en->blood_increase(att_power);
            isfighting=true;
        }
        else
        {
            ta=NULL;
            isfighting=false;
            return;
        }

    }
    else if(ta!=NULL)
    {
        if(this->com_dis(ta)<=att_range&&ta->get_nb()<ta->get_ib())
        {
            ta->blood_increase(att_power);
            isfighting=true;
        }
        else
        {
            ta=NULL;
            isfighting=false;
            return;
        }
    }
    else
    {
        ta=NULL;
        isfighting=false;
        return;
    }
}



