#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include"grid.h"
#include <QMainWindow>
#include<QVector>
#include"unit.h"
#include"enemyunit.h"
#include"myunit.h"
#include<QLabel>
#include"myunitbtn.h"
#include<QTimer>
#include"mylabel.h"
#include"mybtn.h"
class gamewindow : public QMainWindow
{
    Q_OBJECT
    int map[15][15];
    int a_map[15][15];
    Grid* g_map;
    QVector<int *> v_grid;
    QVector<QVector<int *>> v_paths;
    QVector<EnemyUnit*> v_enemy;
    QVector<MyUnit*> v_myunit;
    QVector<myunitbtn*> mbtn;
    QLabel* life=new QLabel(this);
    QVector<QString> imagpath;
    QLabel* sour=new QLabel(this);
    QVector<myLabel*> v_mylabel;
    QVector<int> v_cost;
    QString str;
    mybtn* backbtn;
    mybtn* nextbtn;
    mybtn* rebtn;
    int press_x;
    int press_y;
    int player_blood;
    int player_source;
    int random;
    int grd_counter=0;
    int fly_counter=0;
    int ass_counter=0;
    int grdmaxnum;
    int flymaxnum;
    int wave;
    int round;
    int map_num=3;
    QTimer * timer=new QTimer(this);
    QTimer * timer1=new QTimer(this);
    QTimer * timer2 = new QTimer(this);
    QTimer * timer3= new QTimer(this);
    QTimer * timer4=new QTimer(this);
    bool startg=false;
public:
    explicit gamewindow(int const[][15],int round,QWidget *parent = nullptr);
    void drawmap(QPainter&);
    void drawenemy(QPainter&);
    void drawmyunit(QPainter&);
    void drawbullet(QPainter&);
    void drawattrange(QPainter&);
    void drawsource();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void generate_path();
    QString the_picture(int);
    void path_dfs(int,int,int [][15]);
    void path_dfs1(int,int,int [][15]);
    ~gamewindow();
    void add_grdenemy(int type);//0:地面近程 1：地面远程
    void add_flyenemy(int);
    void add_assenemy();
    void startgame();
    void clearenemy();
    void clearmyunit();
    void muatt();
    void enatt();
    void setButton(myunitbtn*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void checkgamestate();
    void initbackbtn();
    void initnextbtn();
signals:
    void Back();
};

#endif // GAMEWINDOW_H
