#include "mybtn.h"
#include<QDebug>
#include<QPropertyAnimation>
mybtn::mybtn(qreal p1,qreal p2,QString normalImag,QString pressImg)
{
    this->normalImgPath=normalImag;
    this->pressImgpath=pressImg;
    QPixmap pix;
    pix.load(normalImag);
    this ->setFixedSize(pix.width()*p1,pix.height()*p2);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width()*p1,pix.height()*p2));
}
void mybtn::jump1()
{
    QPropertyAnimation * anim = new QPropertyAnimation(this,"geometry");
    anim->setDuration(200);
    anim->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    anim->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    anim->setEasingCurve(QEasingCurve::OutBounce);
    anim->start();
}
void mybtn::jump2()
{
    QPropertyAnimation * anim = new QPropertyAnimation(this,"geometry");
    anim->setDuration(200);
    anim->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
     anim->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
     anim->setEasingCurve(QEasingCurve::OutBounce);
     anim->start();
}
