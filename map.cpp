#include "map.h"
#include <string>
using std::string;

place::place()
{
    x=0;y=0;if_air=0;
    if_dest=0;if_road=0;
    if_birth=0;if_tower=0;
}

place::place(int x,int y,bool tower,bool road,bool air,bool birth,bool dest)
{
    this->x=x;this->y=y;
    if_tower=tower;if_road=road;
    if_air=air;if_birth=birth;if_dest=dest;
    this->occupied=0;
    this->plants_ID=0;
    this->zoombies_id=NULL;
}
road::road(){}
road::road(QString str)
{
    QFile file(str);
    file.open(QIODevice::ReadOnly);//以读文件的方式打开文本
    while(!file.atEnd())//读到文件末尾
    {
        QString temp = file.readLine();
        string buff = temp.toStdString();
        vector<int>list;
        for(int i=0;i<7;i++)//总共由七组数据
        {
            char t=buff[2*i];
            list.push_back(t-'0');
        }
        this->path.push_back(place(list[0],list[1],list[2],list[3],list[4],list[5],list[6]));
    }
    this->ifair=0;
    file.close();
}
air_road::air_road(QString str)
{
    QFile file(str);
    file.open(QIODevice::ReadOnly);//以读文件的方式打开文本
    while(!file.atEnd())//读到文件末尾
    {
        QString temp = file.readLine();
        string buff = temp.toStdString();
        vector<int>list;
        for(int i=0;i<7;i++)//总共由七组数据
        {
            char t=buff[2*i];
            list.push_back(t-'0');
        }
        this->path.push_back(place(list[0],list[1],list[2],list[3],list[4],list[5],list[6]));
    }
    this->ifair=1;
    file.close();
}
myMap::myMap(QString txtpath)
{
    QFile file(txtpath);
    file.open(QIODevice::ReadOnly);
    QString temp=file.readLine();//
    int road_num=temp.toInt();
    //读取陆地路径
    for(int i=0;i<road_num;i++)
    {
        QString roadpath=file.readLine().trimmed();
        road rd(roadpath);rd.index=i;
        this->list.push_back(rd);
    }
    for(int i=0;i<7;i++)
    {
        for(int j=0;j<9;j++)
        {
            this->matrix[i][j]=0;
            this->occupied_matrix[i][j]=0;
        }
    }
    this->only_road=list;
    //读取空中路径
    QString temp3=file.readLine();
    int air_road_num=temp3.toInt();
    for(int i=0;i<air_road_num;i++)
    {
        QString roadpath2=file.readLine().trimmed();
        this->list.push_back(road(roadpath2));
        this->list.back().ifair=1;//将路径改成空中路径
        this->list.back().index=i+road_num;
    }
    //将矩阵matrix中对应格子标记为路径
    for(int j=0;j<road_num;j++)
    {
        for(int i=0;i<(int)this->list[j].path.size();i++)
        {
            this->matrix[this->list[j].path[i].x][this->list[j].path[i].y]=1;
            //qDebug()<<"x的坐标是："<<this->list[j].path[i].x<<"y的坐标是："<<this->list[j].path[i].y;
        }
    }
    while(!file.atEnd())
    {
        QString temp2 = file.readLine();
        string buff = temp2.toStdString();
        int x=buff[0]-'0';int y=buff[2]-'0';
        tower_matrix[x][y]=1;//该点可以种植防御塔
    }
}
