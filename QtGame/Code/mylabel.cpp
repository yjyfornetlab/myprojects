#include "mylabel.h"

myLabel::myLabel(int cost,QMainWindow* parent,int l_x,int l_y)
{
    iconLabel=new QLabel(parent);
    textLabel=new QLabel(parent);
    iconLabel->setGeometry(l_x,l_y,20,20);
    iconLabel->setPixmap(QPixmap(":/tupian/coin.png").scaled(20,20,Qt::AspectRatioMode()));
    textLabel->setFont(QFont("黑体",10));
    textLabel->setText(QString("%1").arg(cost));
    textLabel->setGeometry(l_x+27,l_y,20,20);
    iconLabel->adjustSize();
    textLabel->adjustSize();
}
