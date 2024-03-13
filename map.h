#ifndef MAP_H
#define MAP_H

#include <QFile>
#include <QDebug>
#include <QVector>
#include "charactor.h"
using std::vector;

class place//每个单元格
{
public:
    place();
    place(int x,int y,bool tower,bool road,bool air,bool birth=0,bool dest=0);
    int x,y;//坐标
    bool occupied;
    bool if_tower;
    bool if_road;
    bool if_air;
    bool if_birth;
    bool if_dest;
    int plants_ID;
    int* zoombies_id;
};

class road//路径
{
public:
    road();
    road(QString);//将文件传入构造函数，读取文件内容构建路径
    vector<place> path;
    bool ifair;
    int index;
};

class air_road:road
{
public:
    air_road(){};
    air_road(QString);
    vector<place> path;
};

class myMap
{
public:
    myMap(){};
    myMap(QString path);//一个地图可能存在多条路径
    vector<road>list;//所以路径
    vector <road>only_road;//只可以给陆地僵尸走的路径
    vector<air_road>air_list;//空中路径
    int matrix[10][10];//用来记录哪些格子是路径
    int occupied_matrix[10][10];//哪些格子种植有植物
    int tower_matrix[10][10];//哪些格子可以种植防御塔

};
#endif // MAP_H
