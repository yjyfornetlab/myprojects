#include "gamewindow.h"
#include<QMenuBar>
#include<QPainter>
#include<QVector>
#include<QDebug>
#include<QTimer>
#include<QMouseEvent>
#include"numdefine.h"
#include"mybtn.h"
#include"myunitbtn.h"
#include"time.h"
#include"bullet.h"
#include<QMessageBox>
#include<fstream>
#include"startwindow.h"
using namespace std;
gamewindow::gamewindow(int const m[][15],int round,QWidget *parent) : QMainWindow(parent)
{

    backbtn=new mybtn(0.8,0.8,":/tupian/backtobegin.png");
    backbtn->setParent(this);
    backbtn->move(0,height1-gsize);
    nextbtn=new mybtn(0.8,0.8,":/tupian/next.png");
    nextbtn->setParent(this);
    nextbtn->setVisible(0);
    nextbtn->move(width2-45,height1-45);
    rebtn=new mybtn(0.55,0.55,":/tupian/restart.png");
    rebtn->setParent(this);
    rebtn->move(width2-45,height1-45);
    rebtn->setVisible(0);
    connect(backbtn,&mybtn::clicked,[=](){
        backbtn->jump1();
        backbtn->jump2();
        this->close();
        startWindow* sw=new startWindow;
        sw->show();
    });
    setMouseTracking(true);
    this->round=round;
    imagpath.push_back(":/tupian/clsmy1.png");
    imagpath.push_back(":/tupian/clsmy2.png");
    imagpath.push_back(":/tupian/farmy1.png");
    imagpath.push_back(":/tupian/farmy2.png");
    imagpath.push_back(":/tupian/nut.png");
    imagpath.push_back(":/tupian/spikeweed.png");
    v_cost.push_back(cls1_cost);
    v_cost.push_back(cls2_cost);
    v_cost.push_back(far1_cost);
    v_cost.push_back(far2_cost);
    v_cost.push_back(nut_cost);
    v_cost.push_back(spike_cost);
    int pos_y=10;
    int pos_y1=0;
    for(int i=0;i<mu_num;i++)
    {
        mbtn.push_back(new myunitbtn(imagpath[i],i,width2+10,10+i*(btn_height+gap),this));
        pos_y=pos_y+btn_height+gap;
        pos_y1=10+(i+1)*btn_height+i*gap;
        v_mylabel.push_back(new myLabel(v_cost[i],this,width2+10,pos_y1));
    }
    for(auto ubtn:mbtn)
        setButton(ubtn);
    QPushButton * cancel=new QPushButton(this);
    cancel->setText("取消选择");
    cancel->setFixedSize(80,40);
    cancel->move(width2+10,pos_y);
    connect(cancel,&QPushButton::clicked,this,[=](){
        if(myunitbtn::returnnowbtn()!=NULL)
        {
            myunitbtn::returnnowbtn()->setChecked(false);
            myunitbtn::setnull();
        }
    });
     this->setFixedSize(width1,height1);
     this->setWindowIcon(QPixmap(":/W_icon.jpeg"));
     this->setWindowTitle("game");

     if(round==1)
     {
         player_blood=10;
         player_source=1000;
         wave=3;
         grdmaxnum=3;
         flymaxnum=2;
     }
     else if(round==2)
     {
         player_blood=8;
         player_source=900;
         wave=3;
         grdmaxnum=3;
         flymaxnum=1;
     }
     else if(round==3)
     {
         player_blood=5;
         player_source=750;
         wave=5;
         grdmaxnum=5;
         flymaxnum=2;
     }
     life->move(20,20);
     life->setFont(QFont("黑体",10));
     sour->move(20,40);
     sour->setFont(QFont("黑体",10));
     memcpy(this->map,m,sizeof(this->map));
     memcpy(this->a_map,m,sizeof(this->a_map));
     g_map=new Grid[grid_num1*grid_num2];
     for(int i=0;i<grid_num1;i++)
         for(int j=0;j<grid_num2;j++)
             *(g_map+i*15+j)=Grid(j*gsize,i*gsize,map[i][j]);//坐标
     generate_path();
}
void gamewindow::startgame()
{  
    timer->start(100);
    timer1->start(2000);
    timer2->start(100);
    timer3->start(3000);
    connect(timer,&QTimer::timeout,[=](){
        checkgamestate();
        clearenemy();
        clearmyunit();
        enatt();
        muatt();
        update();
    });
    connect(timer1,&QTimer::timeout,[=](){
            this->startg=true;
            if(ass_counter<1)
            {
                add_assenemy();
                ass_counter++;
            }
            random =rand()%100;
            if(random%2==0)
                add_grdenemy(0);
            else
                add_grdenemy(1);
            grd_counter++;
            if(grd_counter==grdmaxnum)
            {
                timer1->stop();
                grd_counter=0;
                ass_counter=0;
                wave-=1;
                if(wave>0)
                    timer4->start(20000);
            }
    });
    connect(timer2,&QTimer::timeout,[=](){
        for(auto enemy:v_enemy)
            enemy->move();
    });
    connect(timer3,&QTimer::timeout,[=](){
            random=rand()%100;
            if(random%3==0)
                add_flyenemy(0);
            else
                add_flyenemy(1);
            fly_counter++;
            if(fly_counter==flymaxnum)
            {
                timer3->stop();
                fly_counter=0;
            }
    });
    connect(timer4,&QTimer::timeout,[=](){
        if(!timer1->isActive()&&!timer3->isActive())
        {
            timer1->start(2000);
            timer3->start(3000);
        }
        timer4->stop();
    });
}
//0正常格子
//1地面路径格子
//2敌方地面路径起点
//3地面路径终点
//4能够放置近战成员
//5能够放置远程单位
//6敌方飞行单位起点
//7敌方只有飞行单位能经过的格子
//8敌方飞行单位和地面单位都能经过的格子
void gamewindow::generate_path()
{
    int m[grid_num1][grid_num2];
    memcpy(m,this->map,sizeof(m));
    v_grid.clear();
    v_paths.clear();
    for(int i=0;i<grid_num1;i++)
    {
        for(int j=0;j<grid_num2;j++)
        {
            if(this->map[i][j]==2)
            {
                v_grid.clear();
                path_dfs(i,j,m);
            }
            else if(this->map[i][j]==6)
            {
                v_grid.clear();
                path_dfs1(i,j,m);
            }
        }
    }
}
void gamewindow::path_dfs(int x, int y, int (*m)[15])
{
    int direction[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
    v_grid.push_back(new int[2]{x,y});
    for(int k=0;k<4;k++)
    {
        int i=x+direction[k][0];
        int j=y+direction[k][1];
        int t=m[i][j];
        int t1=m[x][y];
        if(i<grid_num1&&j<grid_num2&&(t==1||t==3||t==4||t==8))
        {
            if(t==3)
            {
                v_grid.push_back(new int[2]{i,j});
                v_paths.push_back(v_grid);
                return;
            }
            else
            {
                m[x][y]=-1;
                path_dfs(i,j,m);
                m[x][y]=t1;
            }
        }
    }
    v_grid.pop_back();
}
void gamewindow::path_dfs1(int x, int y, int (*m)[15])
{
    int direction[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
    v_grid.push_back(new int[2]{x,y});
    for(int k=0;k<4;k++)
    {
        int i=x+direction[k][0];
        int j=y+direction[k][1];
        int t=m[i][j];
        int t1=m[x][y];
        if(i<grid_num1&&j<grid_num2&&(t==3||t==7||t==8))
        {
            if(t==3)
            {
                v_grid.push_back(new int[2]{i,j});
                v_paths.push_back(v_grid);
                return;
            }
            else
            {
                m[x][y]=-1;
                path_dfs1(i,j,m);
                m[x][y]=t1;
            }
        }
    }
    v_grid.pop_back();
}
void gamewindow::drawmap(QPainter& p1)
{
    for(int i=0;i<grid_num1;i++)
        for(int j=0;j<grid_num2;j++)
        {
            int x=(g_map+i*grid_num2+j)->returnx();
            int y=(g_map+i*grid_num2+j)->returny();
            int t=(g_map+i*grid_num2+j)->returntype();
            p1.drawPixmap(x,y,gsize,gsize,QPixmap(the_picture(t)));
        }
}
void gamewindow::drawsource()
{
    life->setText(QString("生命值: %1").arg(player_blood));
    sour->setText(QString("金币数: %1").arg(player_source));
    life->adjustSize();
    sour->adjustSize();
}
QString gamewindow::the_picture(int m)
{
    switch(m)
    {
        case 0:
        case 6:
        case 7:
            return ":/tupian/grass1.jpeg";
        case 2:
            return ":/tupian/birth_place.png";
        case 1:
        case 8:
            return ":/tupian/soil1.jpeg";
        case 3:
            return ":/tupian/des.png";
        case 4:
            return ":/tupian/floor.png";
        case 5:
            return ":/tupian/far_place.png";
    }
    return "";
}
void gamewindow::mousePressEvent(QMouseEvent* ev)
{
    if(this->startg)
    {
        press_x=ev->x()/gsize;
        press_y=ev->y()/gsize;
        int x=(g_map+press_y*grid_num2+press_x)->returnx();
        int y=(g_map+press_y*grid_num2+press_x)->returny();
        myunitbtn* temp=myunitbtn::returnnowbtn();
        if(map[press_y][press_x]==4)
        {
            if(temp!=NULL&&player_source>=temp->retcost())
            {
                if(temp->returntype()==0)
                {
                    v_myunit.push_back(new cls_myunit(x,y,0));
                    player_source-=cls1_cost;
                }
                else if(temp->returntype()==1)
                {
                    v_myunit.push_back(new cls_myunit(x,y,1));
                    player_source-=cls2_cost;
                }
                else if(temp->returntype()==4)
                {
                    v_myunit.push_back(new nut(x,y,3));
                    player_source-=nut_cost;
                }
                else
                    return;
                temp->setChecked(false);
                myunitbtn::setnull();
                map[press_y][press_x]=-1;
        }
        else if(temp!=NULL&&player_source<temp->retcost())
        {
            QMessageBox::warning(this,"!","金币数不足");
            return;
        }
        else return;
    }
    else if(map[press_y][press_x]==5)
    {
        if(temp!=NULL&&player_source>=temp->retcost())
        {
            if(temp->returntype()==2)
            {
                v_myunit.push_back(new far_myunit(x,y,0));
                player_source-=far1_cost;
            }
            else if(temp->returntype()==3)
            {
                v_myunit.push_back(new far_myunit(x,y,1));
                player_source-=far2_cost;
            }
            else
                return;
            temp->setChecked(false);
            myunitbtn::setnull();
            map[press_y][press_x]=-1;
        }
        else if(temp!=NULL&&player_source<temp->retcost())
        {
            QMessageBox::warning(this,"!","金币数不足");
            return;
        }
        else return;
    }
    else if(map[press_y][press_x]==1||map[press_y][press_x]==8)
        {

            if(temp!=NULL&&player_source>=temp->retcost())
            {
                if(temp->returntype()==5)
                {
                    v_myunit.push_back(new spikeweed(x,y,4,this));
                    player_source-=spike_cost;
                }
                else
                    return;
                temp->setChecked(false);
                myunitbtn::setnull();
                map[press_y][press_x]=-1;
            }
            else if(temp!=NULL&&player_source<temp->retcost())
            {
                QMessageBox::warning(this,"!","金币数不足");
                return;
            }
            else return;
        }
   }
   else
        return;
}
void gamewindow::mouseMoveEvent(QMouseEvent * ev)
{
    for(auto my_un:v_myunit)
    {
        if(abs(ev->x()-my_un->get_x())<=gsize&&abs(ev->y()-my_un->get_y())<=gsize)
        {
           if(!my_un->ret_rv())
               my_un->set_rv(true);
        }
        else
            my_un->set_rv(false);
    }
    for(auto en:v_enemy)
    {
        if(abs(ev->x()-en->get_x())<=gsize&&abs(ev->y()-en->get_y())<=gsize)
        {
           if(!en->ret_rv())
               en->set_rv(true);
        }
        else
            en->set_rv(false);
    }
}
void gamewindow::mouseDoubleClickEvent(QMouseEvent * ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        for(auto my_un:v_myunit)
        {
            if(abs(ev->x()-my_un->get_x())<=30&&abs(ev->y()-my_un->get_y())<=30)
                QMessageBox::information(this,"单位状态",QString("攻击：%1\n总血量：%2\n剩余血量：%3\n").arg(my_un->ret_power()).arg(my_un->ret_intib()).arg(my_un->ret_nowb()));
        }
    }
}
void gamewindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    drawmap(painter);
    drawenemy(painter);
    drawmyunit(painter);
    drawbullet(painter);
    drawattrange(painter);
    drawsource();
}
void gamewindow::drawenemy(QPainter& p1)
{
    for(auto enemy: v_enemy)
    {
        p1.setPen(Qt::NoPen);
        QBrush red_brush(QColor(255,0,0));
        p1.setBrush(red_brush);
        float portion=1.0*enemy->get_nb()/enemy->get_ib();
        p1.drawRect(enemy->get_x()+(gsize-enemy->get_picw())/2,enemy->get_y()-10+(gsize-enemy->get_pich())/2,portion*enemy->get_picw(),6);
        p1.drawPixmap(enemy->get_x()+(gsize-enemy->get_picw())/2,enemy->get_y()+(gsize-enemy->get_pich())/2,enemy->get_picw(),enemy->get_pich(),QPixmap(enemy->Enemypic()));
    }
}
void gamewindow::drawmyunit(QPainter& p1)
{
    for(auto my_un: v_myunit)
    {
        qreal pic_y;
        if(my_un->get_mutype()==4)
            pic_y=my_un->get_y()+gsize*0.6;
        else
            pic_y=my_un->get_y()+(gsize-my_un->get_pich())/2;
        p1.setPen(Qt::NoPen);
        p1.drawPixmap(my_un->get_x()+(gsize-my_un->get_picw())/2,pic_y,my_un->get_picw(),my_un->get_pich(),QPixmap(my_un->getmy_unpic()));
        QBrush red_brush(QColor(255,0,0));
        p1.setBrush(red_brush);
        float portion=1.0*my_un->get_nb()/my_un->get_ib();
        p1.drawRect(my_un->get_x()+(gsize-my_un->get_picw())/2,pic_y-10,portion*my_un->get_picw(),6);
    }
}
void gamewindow::drawbullet(QPainter& p1)
{
    for(auto my_un: v_myunit)
    {
        if(my_un->get_mutype()==0)
            continue;
        else
        {
            if(my_un->ret_bull()!=NULL)
            {

                Bullet* bull=my_un->ret_bull();
                if(my_un->spec_type()==0)
                    p1.drawPixmap(bull->get_x(),bull->get_y(),bull->ret_picw(),bull->ret_pich(),QPixmap(bull->rpic()));
                else
                {
                    QMatrix matrix;
                    matrix.rotate(bull->cal_angle());
                    p1.drawPixmap(bull->get_x(),bull->get_y(),bull->ret_picw(),bull->ret_pich(),QPixmap(bull->rpic()).transformed(matrix));
                }
            }
        }
    }
}
void gamewindow::drawattrange(QPainter& p1)
{
    p1.setBrush(Qt::NoBrush);
    p1.setPen(QPen(Qt::white));
    for(auto my_un:v_myunit)
    {
        if(my_un->ret_rv())
        {
            if(my_un->get_mutype()==4)
                p1.drawEllipse(QPoint(0.5*gsize+my_un->get_x(),0.8*gsize+my_un->get_y()),my_un->ret_range(),my_un->ret_range());
            else
                p1.drawEllipse(QPoint(0.5*gsize+my_un->get_x(),0.5*gsize+my_un->get_y()),my_un->ret_range(),my_un->ret_range());
        }
    }
    p1.setPen(Qt::NoPen);
    p1.setPen(QPen(Qt::black));
    for(auto en:v_enemy)
    {
        if(en->ret_rv())
            p1.drawEllipse(QPoint(en->get_x()+gsize/2,en->get_y()+gsize-en->get_pich()/2),en->ret_range(),en->ret_range());
    }
}
void gamewindow::add_grdenemy(int type)
{
    for(auto path:v_paths)
    {
        int i=(path)[0][0];
        int j=(path)[0][1];
        if(map[i][j]==2)
        {
            if(type==0)
                v_enemy.push_back(new grd_enemyunit(path,type));
            else if(type==1)
                v_enemy.push_back(new grd_enemyunit(path,type));
        }

    }
}
void gamewindow::add_flyenemy(int type)
{
    for(auto path:v_paths)
    {
        int i=(path)[0][0];
        int j=(path)[0][1];
        if(map[i][j]==6)
               v_enemy.push_back(new fly_enemyunit(path,type));
    }
}
void gamewindow::add_assenemy()
{
    for(auto path:v_paths)
    {
        int i=(path)[0][0];
        int j=(path)[0][1];
        if(map[i][j]==2)
               v_enemy.push_back(new assist_enemy(path));
    }
}
void gamewindow::clearenemy()
{
    for(auto enemy=v_enemy.begin();enemy!=v_enemy.end();)
    {
        if((*enemy)->get_nb()<=0||(*enemy)->at_end())
        {
            if((*enemy)->at_end()){
                if(player_blood>0)
                    player_blood-=1;
            }
            for(auto my_unit:v_myunit)
                my_unit->erase_en(*enemy);
            delete (*enemy);
            enemy= v_enemy.erase(enemy);
        }
        else
            enemy++;
    }
}
void gamewindow::clearmyunit()
{
    for(auto my_unit=v_myunit.begin();my_unit!=v_myunit.end();)
    {
        if((*my_unit)->get_nb()<=0)
        {
            for(auto enemy:v_enemy)
            {
                if(enemy->get_target()==(*my_unit))
                    enemy->set_null();
            }
            if((*my_unit)->get_mutype()==0||(*my_unit)->get_mutype()==3)
                map[(*my_unit)->get_y()/gsize][(*my_unit)->get_x()/gsize]=4;
            else if((*my_unit)->get_mutype()==1)
                map[(*my_unit)->get_y()/gsize][(*my_unit)->get_x()/gsize]=5;
            else if((*my_unit)->get_mutype()==4)
                map[(*my_unit)->get_y()/gsize][(*my_unit)->get_x()/gsize]=1;
            delete (*my_unit);
            my_unit=v_myunit.erase(my_unit);
        }
        else
            my_unit++;
    }
}
void gamewindow::checkgamestate()
{
    QMessageBox::StandardButton stb;
    if(player_blood==0)
    {
        timer->stop();
        timer1->stop();
        timer2->stop();
        timer3->stop();
        timer4->stop();
        stb=QMessageBox::information(this,"game","游戏失败");
        if(stb==QMessageBox::Ok)
        {
            rebtn->setVisible(1);
            connect(rebtn,&mybtn::clicked,[=](){
                gamewindow* g=new gamewindow(a_map,this->round);
                this->close();
                g->show();
                QTimer::singleShot(1000,g,[=](){
                    g->startgame();
                });
            });
        }
    }
    else
    {
        if(wave==0&&v_enemy.isEmpty())
        {
            timer->stop();
            timer1->stop();
            timer2->stop();
            timer3->stop();
            timer4->stop();
            if(round<map_num)
                stb=QMessageBox::information(this,"game","游戏胜利");
            else
                stb=QMessageBox::information(this,"game","已通关");
            if(stb==QMessageBox::Ok)
            {
                if(round<map_num)
                {
                    nextbtn->setVisible(1);
                    connect(nextbtn,&mybtn::clicked,[=](){
                       nextbtn->jump1();
                       nextbtn->jump2();
                       this->close();
                       if(round==1)
                       {
                           int m1[15][15];
                           ifstream fin1("D:\\project_02_1\\map1.txt",ios::in);
                           if(fin1)
                            {
                               for(int i=0;i<grid_num1;i++)
                                   for(int j=0;j<grid_num2;j++)
                                       fin1 >> m1[i][j];
                           }
                           fin1.close();
                           gamewindow* g=new gamewindow(m1,2);
                           g->show();
                           QTimer::singleShot(1000,g,[=](){
                               g->startgame();
                           });
                       }
                       else if(round==2)
                       {
                           int m2[15][15];
                           ifstream fin2("D:\\project_02_1\\map2.txt",ios::in);
                           if(fin2)
                            {
                               for(int i=0;i<grid_num1;i++)
                                   for(int j=0;j<grid_num2;j++)
                                       fin2 >> m2[i][j];
                           }
                           fin2.close();
                           gamewindow* g=new gamewindow(m2,3);
                           g->show();
                           QTimer::singleShot(1000,g,[=](){
                               g->startgame();
                           });
                       }
                    });
                }
            }
        }
        else
            return;
    }
}
void gamewindow::enatt()
{
    for(auto enemy2:v_enemy)
    {
        for(auto my_unit2:v_myunit)
        {
           if(enemy2->meet_myunit(my_unit2))
                enemy2->set_target(my_unit2);
        }
        enemy2->attack();
    }
    for(auto enemy:v_enemy)
    {
        if(enemy->get_etype()==2)
        {
            for(auto enemy1:v_enemy)
                enemy->add_blood(enemy1);
        }
    }
}
void gamewindow::muatt()
{
    for(auto my_unit3:v_myunit)
    {
        for(auto enemy3:v_enemy)
        {

            if(my_unit3->meet_enemy(enemy3))
            {
                my_unit3->insert_tg(enemy3);
                break;
            }
        }
        my_unit3->attack_tg();
    }
}

void gamewindow::setButton(myunitbtn* btn)
{
    connect(btn,&QPushButton::clicked,this,[=](){
        if(btn->returnnowbtn()!=NULL)
        {
            btn->returnnowbtn()->setChecked(false);
            btn->setnowbtn();
        }
        else
            btn->setnowbtn();
    });
}
gamewindow::~gamewindow()
{
    delete []g_map;
    for(auto it:v_grid)
       delete []it;
    for(auto it1:v_paths)
        for(auto it2:it1)
            delete []it2;
}





