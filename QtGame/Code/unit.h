#ifndef UNIT_H
#define UNIT_H
#include<QObject>
class Unit:public QObject
{
protected:
    int state;
    int u_x;
    int u_y;
    int att_power;
    int att_range;
    int pic_height;
    int pic_width;
    bool isfighting;
    int init_blood;
    int now_blood;
    bool range_visible=false;
public:
   Unit();
   int get_x();
   int get_y();
   int get_pich();
   int get_picw();
   int get_ib();
   int get_nb();
   bool& whether_fighting();
   bool ret_rv();
   void set_rv(bool);
   int ret_range();
};


#endif // UNIT_H
