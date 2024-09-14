#ifndef MYUNITBTN_H
#define MYUNITBTN_H
#include"mylabel.h"
#include<QPushButton>
#include<QWidget>
class myunitbtn : public QPushButton
{
    Q_OBJECT
    QString Imagpath;
    static myunitbtn* isclicked;
    int type;
public:
      myunitbtn(QString Imag,int,int x,int y,QWidget*);
      static myunitbtn* returnnowbtn();
      void setnowbtn();
      static void setnull();
      int returntype();
      int retcost();
signals:
};

#endif // MYUNITBTN_H
