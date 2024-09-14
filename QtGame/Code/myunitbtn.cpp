#include "myunitbtn.h"
#include "numdefine.h"
#include"gamewindow.h"
//0:近战1
//1:近战2
//2:远战1
//3:远战2
//4:阻拦
myunitbtn* myunitbtn::isclicked=NULL;
myunitbtn::myunitbtn(QString Imag,int type,int x,int y,QWidget* g)
{
    this->setCheckable(true);
    this->type=type;
    this->Imagpath=Imag;
    QPixmap pix;
    pix.load(this->Imagpath);
    this->setFixedSize(btn_width,btn_height);
    this->setIcon(pix);
    this->setIconSize(QSize(btn_width,btn_height));
    this->setParent(g);
    this->move(x,y);
}
myunitbtn* myunitbtn::returnnowbtn()
{
    return isclicked;
}
void myunitbtn::setnowbtn()
{
    isclicked=this;
}
void myunitbtn::setnull()
{
    isclicked=NULL;
}
int myunitbtn::returntype()
{
    return type;
}
int myunitbtn::retcost()
{
    if(type==0)
        return cls1_cost;
    else if(type==1)
        return cls2_cost;
    else if(type==2)
        return far1_cost;
    else if(type==3)
        return far2_cost;
    else if(type==4)
        return nut_cost;
    else if(type==5)
        return spike_cost;
}
