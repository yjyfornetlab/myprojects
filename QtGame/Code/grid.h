#ifndef GRID_H
#define GRID_H


class Grid
{
    int g_x;
    int g_y;
    int type;
public:
    Grid();
    Grid(int,int);
    Grid(int,int,int);
    int& returntype();
    int& returnx();
    int& returny();
    int returntype() const;
    int returnx() const;
    int returny() const;
};

#endif // GRID_H
