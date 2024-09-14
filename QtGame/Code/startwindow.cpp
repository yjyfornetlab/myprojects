#include "startwindow.h"
#include "ui_startwindow.h"
#include<QPainter>
#include"mybtn.h"
#include<QTimer>
#include<synchapi.h>
#include<QFile>
#include<QTextCodec>
#include<QDebug>
#include<iostream>
#include<fstream>
#include"numdefine.h"
using namespace std;
#define w_width 750
#define w_height 750
//0正常格子
//1地面路径格子
//2敌方地面路径起点
//3地面路径终点
//4能够放置近战成员
//5能够放置远程单位
//6敌方飞行单位起点
//7敌方只有飞行单位能经过的格子
//8敌方飞行单位和地面单位都能经过的格子
startWindow::startWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::startWindow)
{
    ui->setupUi(this);
    setFixedSize(w_width,w_height);
    setWindowIcon(QIcon(":/tupian/W_icon.jpeg"));
    setWindowTitle("塔防小游戏");
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });
    mybtn * startBtn = new mybtn(0.9,0.45,":/tupian/startbtn1.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.8);
    ifstream fin("D:\\project_02_1\\map.txt",ios::in);
    if(fin)
     {
        for(int i=0;i<grid_num1;i++)
            for(int j=0;j<grid_num2;j++)
                fin >> m[i][j];
    }
    fin.close();
    connect(startBtn,&mybtn::clicked,[=](){
            startBtn->jump1();
            startBtn->jump2();
            gamewindow* g=new gamewindow(m,1);
            QTimer::singleShot(500,this,[=](){
                this ->close();
                g->show();
                QTimer::singleShot(1000,g,[=](){
                    g->startgame();
                });
            });
    });
}
void startWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/tupian/start_w1.jpeg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
startWindow::~startWindow()
{
    delete ui;
}

