#include "charactor.h"
#include <QRandomGenerator>

charactor::charactor()
{

}

plants::plants()
{

}
bullet::bullet(int x,int y,int value)
{
    this->x=x;
    this->y=y;
    this->value=value;
}


Bullet::Bullet(int x,int y,int dx,int dy)
{
    this->value=1000;
    this->x=x;
    this->y=y;
    this->dx=dx;
    this->dy=dy;
}

Bullet::~Bullet()
{
    delete this->label2;
}


void Bullet::move(QWidget* par)
{
    this->label2 = new QLabel(par);
    this->label2->setPixmap(QPixmap(this->image_path));
    QPropertyAnimation *a=new QPropertyAnimation(this->label2,"geometry");
    this->label2->show();
    a->setDuration(1500);
    a->setStartValue(QRect(x,y,40,40));
    a->setEndValue(QRect(dx,dy,40,40));
    a->start();
}

lighting::lighting(int x,int y, int dx,int dy):Bullet(x,y,dx,dy)
{
    this->type=2;
}

gas::gas(int x,int y,int dx,int dy):Bullet(x,y,dx,dy)
{
    this->image_path=":/duqi.png";
    this->type=1;
}
void bullet::move()
{
    if(dir==1)//朝右攻击
    {
        this->ox+=4;
    }
    if(dir==2)//朝左攻击
    {
        this->ox-=4;
    }
}
peas::peas(int x,int y,int value,int ox,int oy,int dir,int atk):bullet(x,y,value)
{
    this->dir=dir;this->atk=atk;
    this->value=1;
    this->atk+=500;
    if(dir==1)
    {
    vol=QRect(this->ox+50,this->oy+30,50,50);
    }
    else
    {
        vol=QRect(this->ox-50,this->oy-30,-50,-50);
    }
    type=3;
    this->x=x;
    this->y=y;
    this->ox=ox;
    this->oy=oy;
    this->image_path=":/ress/peabull.png";
}
duqi::duqi(int x,int y,int value,int ox,int oy,QString path):bullet(x,y,value)
{
    type=1;
    this->x=x;
    this->y=y;
    this->ox=ox;
    this->oy=oy;
    this->image_path=path;
}
light::light(int x,int y,int value,int sx,int y2):bullet(x,y,value)
{
    type=2;
    this->ox=sx;
    this->oy=y2;
}
Sett::Sett(int x, int y)
{
    this->x=x;this->y=y;
    this->if_alive=1;
    this->image_path=":/sett.png";
    this->ID=present;present++;
    this->life_value=2000;//生命值
    this->attack_value=200;//攻击力
    this->attack_range=0;//是否长手
    this->if_airatk=0;//是否攻击飞行单位
    this->total_blood=2000;
}

boluo::boluo(int x,int y)
{
    this->x=x;this->y=y;
    this->if_alive=1;
    this->image_path=":/boluo.png";
    this->ID=present;present++;
    this->life_value=10000;//生命值
    this->attack_value=10;//攻击力
    this->attack_range=0;//是否长手
    this->if_airatk=0;//是否攻击飞行单位
    this->total_blood=10000;
    type=1;value = 50;this->vol=QRect(this->y,this->x,80,80);
}
youyu::youyu(int x,int y)
{
    this->x=x;this->y=y;
    this->if_alive=1;
    this->image_path=":/youyu.png";
    this->ID=present;present++;
    this->life_value=500;//生命值
    this->attack_value=150;//攻击力
    this->attack_range=1;//是否长手
    this->if_airatk=0;//是否攻击飞行单位
    this->total_blood=500;
    type=2;value=200;
    this->vol1=QRect(this->y-65,this->x-65,230,230);
    this->vol=QRect(this->y,this->x,95,95);
}
pea::pea(int x,int y,int d)
{
    this->x=x;this->y=y;
    this->if_alive=1;
    this->image_path=":/ress/pea.png";
    this->ID=present;present++;
    this->life_value=500;//生命值
    this->attack_value=50000;//攻击力
    this->attack_range=1;//是否长手
    this->if_airatk=0;//是否攻击飞行单位
    this->total_blood=500;
    type=4;value=100;second=3;
    switch (d) {
    case 1://朝右攻击
        this->vol1=QRect(this->y+20,this->x+35,400,20);
        break;
    case 2://朝左攻击
        this->vol1=QRect(this->y+20,this->x+35,-400,20);
        break;
    default:
        break;
    }
    this->vol=QRect(this->y,this->x,95,95);
}
luwei::luwei(int x,int y)
{
    this->x=x;this->y=y;
    this->if_alive=1;
    this->image_path=":/ress/luwei.png";
    this->ID=present;present++;
    this->life_value=400;//生命值
    this->attack_value=200;//攻击力
    this->attack_range=1;//是否长手
    this->if_airatk=1;//是否攻击飞行单位
    this->total_blood=400;
    type=3;value=200;
    this->vol1=QRect(this->y-130,this->x-130,600,600);
    this->vol=QRect(this->y,this->x,95,95);
    second = 2;
}
zoombies::zoombies()
{

}

normal_zoombies::normal_zoombies(int x, int y)
{
    this->x=x;this->y=y;
    this->image_path=":/enemy2.png";
    this->dying_path=":/dying1.png";
    this->bury_path=":/buried1.png";

    this->eating_path1=":/ress/eating1.png";
    this->eating_path2=":/ress/eating2.png";
    this->eating_pathL=":/ress/eating1L.png";
    this->eating_pathL2=":/ress/eating2L.png";

    this->nor_path=":/enemy2.png";
    this->nor_path_forL=":/enemyleft.png";

    this->nor_path2=":/ress/enemy1.png";
    this->nor_path_forL2=":/ress/enemy1L.png";

    this->dying_path1=":/dying1.png";
    this->bury_path1=":/buried1.png";

    this->dying_forL=":/dying2.png";
    this->bury_forL=":/buried2.png";

    this->ID=present2;present2++;
    this->life_value=5000;//生命值
    this->attack_value=200;//攻击力
    this->if_air=0;
    this->if_alive=1;
    this->index_in_path=0;
    this->pathEnum=0;
    this->if_struck=0;
    this->total_blood=5000;
    this->vol2=QRect(this->y,this->x,80,80);
    this->type = 1;changing=0;
    range=0;
}
fire_zoombies::fire_zoombies(int x,int y)
{
    this->x=x;this->y=y;
    this->image_path=":/ress/fireNormal.png";
    this->dying_path=":/dying1.png";
    this->bury_path=":/buried1.png";

    this->eating_path1=":/ress/fireNormal.png";
    this->eating_path2=":/ress/fireNormal.png";
    this->eating_pathL=":/ress/fireNormalL.png";
    this->eating_pathL2=":/ress/fireNormalL.png";
    this->nor_path=":/ress/fireNormal.png";
    this->nor_path_forL=":/ress/fireNormalL.png";

    this->nor_path2=":/ress/fireNormal.png";
    this->nor_path_forL2=":/ress/fireNormalL.png";

    this->dying_path1=":/dying1.png";
    this->bury_path1=":/buried1.png";

    this->dying_forL=":/dying2.png";
    this->bury_forL=":/buried2.png";

    this->ID=present2;present2++;
    this->life_value=1000;//生命值
    this->attack_value=20;//攻击力
    this->if_air=0;
    this->if_alive=1;
    this->index_in_path=0;//僵尸在路径中的位置，初始为0
    this->pathEnum=0;
    this->if_struck=0;
    this->total_blood=1000;
    this->vol2=QRect(this->y,this->x,80,80);
    this->damage_rect=QRect(this->y-65,this->x-65,230,230);
    this->type = 3; range=0;
    changing=0;
}

balloon_zoombies::balloon_zoombies(int x,int y)
{
    this->x=x;this->y=y;
    this->image_path=":/balloon.png";
    this->dying_path=":/dying1.png";
    this->bury_path=":/buried1.png";


    this->nor_path=":/balloon.png";
    this->nor_path_forL=":/ress/balloonL.png";

    this->nor_path2=":/balloon.png";
    this->nor_path_forL2=":/ress/balloonL.png";

    this->dying_path1=":/dying1.png";
    this->bury_path1=":/buried1.png";

    this->dying_forL=":/dying2.png";
    this->bury_forL=":/buried2.png";

    this->ID=present2;present2++;
    this->life_value=1000;//生命值
    this->attack_value=200;//攻击力
    this->if_air=1;
    this->if_alive=1;
    this->index_in_path=0;//僵尸在路径中的位置，初始为0
    this->pathEnum=0;
    this->if_struck=0;
    this->total_blood=1000;
    this->vol2=QRect(this->y,this->x,80,80);
    this->type = 2;changing=0; range=1;
}
