#ifndef CHARACTOR_H
#define CHARACTOR_H
#include <QString>
#include <QLabel>
#include <QObject>
#include "map.h"
#include <QVector>
#include <QPropertyAnimation>
#include <QRect>
using std::vector;

class charactor
{
public:
    charactor();
};

class Bullet
{
public:
    Bullet(int x,int y,int dx,int dy);
    ~Bullet();
    int x,y,dx,dy;
    int value;
    int type;
    void move(QWidget*);
    QString image_path;
    QLabel* label2;
};

class gas:public Bullet
{
public:
    gas(int x,int y,int dx,int dy);
};

class lighting:public Bullet
{
public:
    lighting(int x,int y,int dx,int dy);
};
class bullet//子弹类
{
public:
    bullet(int x,int y,int value);
    QString image_path; QLabel* labl;
    QRect vol;
    void move();
    int value;//子弹的血量
    int type;//子弹的类型
    int dir;//子弹的方向
    int atk;
    int x,y;
    int ox;
    int oy;
};
class peas:public bullet
{
public:
    peas(int x,int y,int value,int ox,int oy,int dir,int atk);
};

class duqi:public bullet
{
public:
    duqi(int x,int y,int value,int ox ,int oy,QString path);
};

class light:public bullet
{
public:
    light(int x,int y,int value,int shooterx,int);

};

class plants
{
public:
    plants();
    static int present;
    vector<bullet>clip;
    vector<Bullet*>clips;
    QString image_path;
    int x;int y;
    int ID;
    int life_value;//生命值
    int attack_value;//攻击力
    int total_blood;
    int attack_range;
    bool if_airatk;//是否攻击飞行单位
    bool if_alive=1;
    int type;
    int value;
    int second;
    int unit_x;
    int unit_y;
    int direction;
    QRect vol1;
    QRect vol;
};


class Sett:public plants//瑟提继承植物类
{
public:
    Sett(int x, int y);//表示坐标
};

class boluo:public plants
{
public:
    boluo(int x,int y);
};
class youyu:public plants
{
public:
    youyu(int x,int y);
};


class pea:public plants
{
public:
    pea(int x,int y,int);//初始化的时候需要有一个方向
};

class icepea:public plants
{
public:
    icepea(int x,int y,int);//初始化的时候需要有一个方向
};

class luwei:public plants
{
public:
    luwei(int x,int y);
};

class zoombies
{
public:
    zoombies();
    static int present2;
    QString image_path;//绘画使用的图片
    QString dying_path;//僵尸半跪的图片
    QString bury_path;//僵尸倒地的图片

    QString nor_path;//正常图片
    QString nor_path2;//正常的第二张图片

    QString dying_path1;//僵尸半跪的图片
    QString bury_path1;//僵尸倒地的图片

    QString eating_path1;
    QString eating_path2;

    QString nor_path_forL;
    QString nor_path_forL2;

    QString dying_forL;
    QString bury_forL;

    QString eating_pathL;
    QString eating_pathL2;

    int x;int y;
    int ID;
    int life_value;//生命值
    int total_blood;
    int attack_value;//攻击力
    bool if_air;
    bool if_alive=1;
    int pathEnum;
    int index_in_path;
    bool if_struck;
    bool if_buried;
    int type;
    int changing;
    bool range;

    QRect vol2;
    QRect buff_rect;
    QRect damage_rect;
};

class normal_zoombies:public zoombies
{
public:
    normal_zoombies(int x,int y);
};

class balloon_zoombies:public zoombies
{
public:
    balloon_zoombies(int x,int y);
};

class fire_zoombies:public zoombies
{
public:
    fire_zoombies(int x,int y);
};

class flag_zoombies:public zoombies
{
public:
    flag_zoombies(int x,int y);
};

#endif // CHARACTOR_H
