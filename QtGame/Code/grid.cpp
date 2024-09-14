#include "grid.h"

Grid::Grid()
{
    g_x=0;
    g_y=0;
    type=0;
}
Grid::Grid(int g_x,int g_y)
{
    this->g_x=g_x;
    this->g_y=g_y;
    this->type=0;
}
Grid::Grid(int g_x,int g_y,int type)
{
    this->g_x=g_x;
    this->g_y=g_y;
    this->type=type;
}
int& Grid::returntype()
{
    return type;
}
int& Grid::returnx()
{
    return g_x;
}
int& Grid::returny()
{
    return g_y;
}
int Grid::returnx()const
{
    return g_x;
}
int Grid::returny()const
{
    return g_y;
}
int Grid::returntype()const
{
    return type;
}

