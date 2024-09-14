#include "unit.h"

Unit::Unit()
{
    u_x=0;
    u_y=0;
    att_power=0;
    isfighting=false;
}
int Unit::get_x()
{
    return u_x;
}
int Unit::get_y()
{
    return u_y;
}
int Unit::get_pich()
{
    return pic_height;
}
int Unit::get_picw()
{
    return pic_width;
}
int Unit::get_ib()
{
    return init_blood;
}
int Unit::get_nb()
{
    return now_blood;
}
bool& Unit::whether_fighting()
{
    return isfighting;
}
bool Unit::ret_rv()
{
    return range_visible;
}
void Unit::set_rv(bool flag)
{
    this->range_visible=flag;
}
int Unit::ret_range()
{
    return att_range;
}
