#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include<QMainWindow>
class myLabel
{
    QLabel* iconLabel;
    QLabel* textLabel;
    int cost;
public:
    myLabel(int cost,QMainWindow* parent,int l_x,int l_y);
};

#endif // MYLABEL_H
