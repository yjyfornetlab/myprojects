#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include<QVector>
#include <QMainWindow>
#include"gamewindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class startWindow; }
QT_END_NAMESPACE

class startWindow : public QMainWindow
{
    Q_OBJECT
    int m[15][15];
    int map_num=3;
public:
    startWindow(QWidget *parent = nullptr);
    ~startWindow();
    void paintEvent(QPaintEvent*);
private:
    Ui::startWindow *ui;
};
#endif // STARTWINDOW_H
