#include "bullet.h"
#include"numdefine.h"
#include<QtMath>
#include<QDebug>
Bullet::Bullet(double b_x,double b_y,int speed,int type)
{
    this->b_x=b_x;
    this->b_y=b_y;
    this->speed=speed;
    this->target=NULL;
    this->type=type;
    if(this->type==0)
    {
        this->pic_width=0.2*gsize;
        this->pic_height=0.2*gsize;
    }
    else if(this->type==1)
    {
        this->pic_width=0.8*gsize;
        this->pic_height=0.3*gsize;
    }
}
void Bullet::settarget(EnemyUnit *eu)
{
    this->target=eu;
}
void Bullet::move()
{
    if(this->b_x==target->get_x())
    {
        if(this->b_y<target->get_y())
            this->b_y+=speed;
        else
            this->b_y-=speed;
    }
    else
    {
        this->k=(this->b_y-target->get_y())/(this->b_x-target->get_x());
        if(this->b_x>target->get_x())
         {
            this->b_x-=speed;
            this->b_y-=k*speed;
         }
         else
         {
            this->b_x+=speed;
            this->b_y+=k*speed;
         }
    }
}
double Bullet::get_x()
{
    return b_x;
}
double Bullet::get_y()
{
    return b_y;
}
int Bullet::ret_picw()
{
    return pic_width;
}
int Bullet::ret_pich()
{
    return pic_height;
}
QString Bullet::rpic()
{
    if(this->type==0)
       return ":/tupian/bullet_1.png";
    else if(this->type==1)
    {
        if(b_x==target->get_x())
        {
            if(b_y>target->get_y())
                return ":/tupian/arrow1_3.png";
            else
                return ":/tupian/arrow1_2.png";
        }
        else
        {
            if(b_x<target->get_x())
                return ":/tupian/arrow1_1.png";
            else
                return ":/tupian/arrow1_4.png";
        }
    }
}
bool Bullet::reachtarget()//坐标系统有问题
{
    double distance;
    distance=qSqrt((target->get_x()-b_x)*(target->get_x()-b_x)+(target->get_y()-b_y)*(target->get_y()-b_y));
    if(distance<gsize)
        return true;
    else
        return false;
}
EnemyUnit* Bullet::ret_tg()
{
    return target;
}
qreal Bullet::cal_angle()
{
    if(target->get_x()==b_x)
        return 0;
    qreal raw=qAtan(k);
    qreal angle=raw*180/M_PI;
    if(target->get_x()<b_x&&target->get_y()<b_y)
        angle=360-angle;
    return angle;
}
