#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <QVector>
#include "map.h"
#include "charactor.h"
#include "QTimerEvent"
#include <QMouseEvent>
#include <QMap>
using std::vector;
using std::map;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class myMap;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    vector<QTimer*> timer_list;
    static int goingon;
    static int food;
    static int prePlants;
    static int plant_type;
    static int clear_zms;
    static int newzm_speed;
    static int buliding_pea;
    static int first_x;
    static int has_start;
    static int first_y;
    //static QString origin;
    myMap now_map;
    MainWindow(QWidget *parent = nullptr);
    int food_id;//资源增加的timerid
    int zoombies_timerID;//僵尸移动的timerid
    int zoombies_newID;//僵尸生成的timerid
    int attack_id;//回合攻击的timerid
    int bullet_move_id;
    static int blood;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void draw_myMap(myMap* tmap=NULL);
    void newPlants(int type=0,int x=0,int y=0);//创建一个新的植物
    void newzoombies(int type=1,int x=0,int y=0);
    void attack();
    void remove(int x, int y);
    void bullet_move();//子弹移动的函数
    static bool mouse_has_plant;
    static bool removing;
    vector<plants*>p_list;
    vector<zoombies*>z_list;
    vector <road>paths;
    vector <road>only_road;
    vector<plants*>tower;
    vector <zoombies*>long_zm;
//    map<int,plants>p_map;
//    map<int,zoombies>z_map;
    void move(vector<zoombies*>z_list);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *,QEvent*);
    int find_Plant_byID(int id);
    int find_zoombies_byID(int id);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
