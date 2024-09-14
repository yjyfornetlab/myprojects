#include "myunit.h"
#include<QVector>
#include"numdefine.h"
#include<QDebug>
MyUnit::MyUnit()
{

}
MyUnit::MyUnit(int x,int y,int mu_type)
{
    state=1;
    this->u_x=x;
    this->u_y=y;
    this->mu_type=mu_type;
    bull=NULL;
    target=NULL;
}
int MyUnit::get_mutype()
{
    return mu_type;
}
void MyUnit::attack_tg()
{
    if(this->target!=NULL)
    {
        isfighting=true;
        this->target->get_hurt(att_power);
    }
    else
    {
        isfighting=false;
        return;
    }
}
void MyUnit::insert_tg(EnemyUnit* eu)
{
   this->target=eu;
}
void MyUnit::get_hurt(int hurt)
{
    now_blood-=hurt;
}
void MyUnit::erase_en(EnemyUnit* en)
{
      if(target==en)
          target=NULL;
      if(bull!=NULL)
      {
          if(bull->ret_tg()==en)
          {
             delete bull;
             bull=NULL;
          }
      }
      return;
}


Bullet* MyUnit::ret_bull()
{
    return bull;
}
int MyUnit::ret_power()
{
    return att_power;
}
int MyUnit::ret_intib()
{
    return init_blood;
}
int MyUnit::ret_nowb()
{
    return now_blood;
}
MyUnit::~MyUnit()
{

}


//近战单位
cls_myunit::cls_myunit(int x,int y,int cls_type):MyUnit(x,y,0)
{
    this->cls_type=cls_type;
    if(cls_type==0)
    {
        init_blood=100;
        now_blood=100;
        pic_width=gsize*0.4;
        pic_height=gsize*0.7;
        att_power=10;
        att_range=gsize;
    }
    else if(cls_type==1)
    {
        pic_width=gsize*0.4;
        pic_height=gsize*0.7;
        init_blood=150;
        now_blood=150;
        att_power=20;
        att_range=2.5*gsize;
    }
}
bool cls_myunit::meet_enemy(EnemyUnit* en)
{
    if(cls_type==0)
    {
        if((en->get_etype()==0||en->get_etype()==2)&&en->distance(this)<=att_range)
            return true;
        else
            return false;
    }
    else if(cls_type==1)
    {
        if(en->distance(this)<=att_range)
            return true;
        else
            return false;
    }
    else
        return false;
}
QString cls_myunit::getmy_unpic()
{
    QString picpath;
    if(isfighting)
    {
        if(state==1)
        {
            if(cls_type==0)
               picpath=":/tupian/cls0fight1.png";
            else if(cls_type==1)
                picpath=":/tupian/cls1fight1.png";
            state=2;
        }
        else if(state==2)
        {
            if(cls_type==0)
                picpath=":/tupian/cls0fight2.png";
            else if(cls_type==1)
                picpath=":/tupian/cls1fight2.png";
            state=3;
        }
        else if(state==3)
        {
            if(cls_type==0)
                picpath=":/tupian/cls0fight3.png";
            else if(cls_type==1)
                picpath=":/tupian/cls1fight3.png";
            state=1;
        }
    }
    else
    {
        if(cls_type==0)
            picpath=":/tupian/clsmy1.png";
        else if(cls_type==1)
            picpath= ":/tupian/clsmy2.png";
        state=1;
    }
    return picpath;
}
int cls_myunit::spec_type()
{
    return cls_type;
}

//远程单位
far_myunit::far_myunit(int x,int y,int type):MyUnit(x,y,1)
{
    far_type=type;
    if(far_type==0)
    {
        init_blood=150;
        now_blood=150;
        att_range=3*gsize;
        att_power=10;
        pic_width=gsize*0.8;
        pic_height=gsize*0.5;
    }
    else if(far_type==1)
    {
        init_blood=200;
        now_blood=200;
        att_range=3*gsize;
        att_power=20;
        pic_width=gsize*0.6;
        pic_height=gsize*0.6;
    }
}
bool far_myunit::meet_enemy(EnemyUnit* en)
{
    if(far_type==0)
    {
        if((en->get_etype()==0||en->get_etype()==2)&&en->distance(this)<=att_range)
            return true;
        else
        {
            target=NULL;
            return false;
        }
    }
    else if(far_type==1)
    {
        if(en->distance(this)<=att_range)
            return true;
        else
        {
            target=NULL;
            return false;
        }
    }
    else
        return false;
}
void far_myunit::attack_tg()
{
    if(target!=NULL)
    {
        if(bull==NULL)
        {
           if(far_type==0)
              bull=new Bullet(this->u_x,this->u_y,10,0);
           else if(far_type==1)
              bull=new Bullet(this->u_x,this->u_y,8,1);
           bull->settarget(this->target);
        }
    }
    if(bull!=NULL)
    {
        if(bull->reachtarget())
        {
            bull->ret_tg()->get_hurt(att_power);
            delete bull;
            bull=NULL;
        }
        else if(bull->get_x()<0||bull->get_y()<0||bull->get_x()>width1||bull->get_y()>height1)
        {
            delete bull;
            bull=NULL;
        }
        else
           bull->move();
    }
}
int far_myunit::spec_type()
{
    return far_type;
}
QString far_myunit::getmy_unpic()
{
    if(far_type==0)
        return ":/tupian/farmy1.png";
    else if(far_type==1)
        return ":/tupian/farmy2.png";
    else
        return"";
}

//阻拦单位
nut::nut(int x,int y,int type):MyUnit(x,y,type)
{
    this->pic_width=gsize*0.6;
    this->pic_height=gsize*0.8;
    this->att_power=0;
    this->init_blood=1000;
    this->now_blood=1000;
    this->att_range=0;
}
QString nut::getmy_unpic()
{
    return ":/tupian/nut.png";
}
int nut::spec_type()
{
    return 0;
}
 bool nut::meet_enemy(EnemyUnit*)
 {
     return false;
 }

 //阻拦数为0的单位
spikeweed::spikeweed(int x,int y,int type,QMainWindow* parent):MyUnit(x,y,type)
{
    this->pic_width=gsize*0.8;
    this->pic_height=gsize*0.4;
    this->att_power=10;
    this->init_blood=300;
    this->now_blood=300;
    this->att_range=0.4*gsize;
    this->bloodtimer=new QTimer(parent);
    bloodtimer->start(100);
    connect(bloodtimer,&QTimer::timeout,parent,[=](){
        now_blood-=5;
    });
}
QString spikeweed::getmy_unpic()
{
    return ":/tupian/spikeweed.png";
}
int spikeweed::spec_type()
{
    return 0;
}
bool spikeweed::meet_enemy(EnemyUnit * en)
{
    if((en->get_etype()==0||en->get_etype()==2)&&en->distance(this)<=att_range){
        return true;
    }
    else
    {
        target=NULL;
        return false;
    }
}


