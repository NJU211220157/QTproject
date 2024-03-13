#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QPainter>
#include <QFileDialog>
#include <QEvent>
#include <QPoint>
#include <QPaintEvent>
#include <QRandomGenerator>

using std::vector;

int MainWindow::food =2000;//游戏的资源，即“阳光”
int plants::present=1;//目前植物已种植的数量，仅仅适用于阶段一
int MainWindow::prePlants=1;//
int zoombies::present2=1;
int MainWindow::clear_zms=0;
int MainWindow::newzm_speed=5000;
int MainWindow::buliding_pea=0;
int MainWindow::first_x=0;
int MainWindow::first_y=0;
int MainWindow::has_start=0;
int x_for_build=0;
int y_for_build=0;
bool MainWindow::mouse_has_plant=0;//是否点击了植物卡槽
bool MainWindow::removing=0;//是否点击了铲子
int MainWindow::plant_type=1;//点击了哪一个植物卡槽
int MainWindow::blood=21;//玩家当前血量
int MainWindow::goingon=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("Plants VS Zoombies");
    ui->setupUi(this);
    setFixedSize(1400,725);
    QString origin;
    QString p;
    ui->pushButton->hide();
    connect(ui->pushButton,&QPushButton::clicked,[=](){
       QString path = QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\86182\\Desktop\\地图image");
    });
    origin="C:/Users/86182/Desktop/QTtest/00_Project/mapfile/map1.txt";
    myMap* map = new myMap(origin);//读取地图
    this->now_map=*map;
    this->paths=now_map.list;

    ui->boluo_label->setPixmap(QPixmap(":/boluo.png"));//
    ui->boluo_label->installEventFilter(this);//安装事件过滤器
    ui->youyu_label->setPixmap(QPixmap(":/youyu.png"));
    ui->youyu_label->installEventFilter(this);//安装事件过滤器
    ui->shovel_label->setPixmap(QPixmap(":/Shovel.png"));
    ui->shovel_label->installEventFilter(this);//安装事件过滤器
    ui->luwei_label->setPixmap(QPixmap(":/ress/luwei.png"));
    ui->luwei_label->installEventFilter(this);
    ui->pea_label->setPixmap(QPixmap(":/ress/pea.png"));
    ui->pea_label->installEventFilter(this);

    //newzoombies();
    ui->blood_label->setText("血量：21");
    QTimer *food_timer=new QTimer(this);food_timer->start(500);
    food_id = food_timer->timerId();//资源增加的计时器
    connect(food_timer,&QTimer::timeout,this,[=](){
        QString str ="资源：";food +=5;
        str+=QString::number(food);
        ui->food_label->setText(str);
    });timer_list.push_back(food_timer);
    QTimer *zmmoving_timer = new QTimer (this);zmmoving_timer->start(20);
    zoombies_timerID = zmmoving_timer->timerId();//僵尸移动的计时器
    connect(zmmoving_timer,&QTimer::timeout,this,[=](){
        move(this->z_list);//调用move()函数，实现每个僵尸的移动
        update();
    });timer_list.push_back(zmmoving_timer);

    QTimer *newzm_timer = new QTimer(this);newzm_timer->start(newzm_speed);
    zoombies_newID=newzm_timer->timerId();//生成僵尸
    connect(newzm_timer,&QTimer::timeout,this,[=](){
        if(newzm_speed>=4000)
        {
            this->newzoombies();
        }
        else
        {
            int a=QRandomGenerator::global()->bounded(100)+1;
            //qDebug()<<a;
            if(a%2==0)
            {this->newzoombies();}
        }
        if(newzm_speed>=700)
        {
            newzm_speed-=100;
        }
        newzm_timer->start(newzm_speed);
    });timer_list.push_back(newzm_timer);

    QTimer *attack_timer=new QTimer(this);attack_timer->start(500);
    attack_id=attack_timer->timerId();//僵尸攻击的ID
    connect(attack_timer,&QTimer::timeout,this,[=](){
        attack();
        update();
    });timer_list.push_back(attack_timer);

    QTimer *bullet_timer=new QTimer(this);bullet_timer->start(1000);
    bullet_move_id=bullet_timer->timerId();//
    connect(bullet_timer,&QTimer::timeout,this,[=](){
        for(int i=0;i<(int)tower.size();i++)
        {
            for(int j=0;j<(int)tower[i]->clips.size();j++)
            {
                delete tower[i]->clips[j];
            }
            tower[i]->clips.clear();
        }
    });timer_list.push_back(bullet_timer);
    for(int i=0;i<(int)timer_list.size();i++)
    {
        timer_list[i]->stop();
    }
    connect(ui->pause_btn,&QPushButton::clicked,[=](){
        if(goingon)
        {
            for(int i=0;i<(int)timer_list.size();i++)
            {
                timer_list[i]->stop();
            }
            ui->pause_btn->setText("继续游戏");
            goingon=0;
        }
        else
        {
            for(int i=0;i<(int)timer_list.size();i++)
            {
                timer_list[i]->start();
            }
            ui->pause_btn->setText("暂停游戏");
            goingon=1;
        }
    });
}

bool MainWindow::eventFilter(QObject * obj,QEvent* ev)
{
    if(!goingon)
    {
        return QMainWindow::eventFilter(obj,ev);
    }
    if(obj == ui->boluo_label)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {
            this->plant_type=1;
            this->mouse_has_plant=1;//当前鼠标有植物，且该植物为boluo
            return true;
        }
    }
    if(obj == ui->youyu_label)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {
            this->plant_type=2;
            this->mouse_has_plant=1;//当前鼠标有植物，且该植物为忧郁蘑菇
            return true;
        }
    }
    if(obj == ui->shovel_label)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {
            this->removing=1;
            return true;
        }
    }
    if(obj == ui->luwei_label)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {
            this->plant_type=3;
            this->mouse_has_plant=1;//当前鼠标有植物，且该植物为忧郁蘑菇
            return true;
        }
    }
    if(obj == ui->pea_label)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {
            this->plant_type=4;
            this->mouse_has_plant=1;//当前鼠标有植物，且该植物为忧郁蘑菇
            return true;
        }
    }
    return QMainWindow::eventFilter(obj,ev);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
//用来判断种植植物
{
    if(this->buliding_pea&&event->x()<1200&&event->x()>250)//想要种植豌豆射手
    {
        int x=first_y;
        int y=first_x;
        int x_unit=(x-135)/75;
        int y_unit=(y-280)/75;
        x=x_unit*75+135-20;//竖直坐标
        y=y_unit*75+280-10;//横轴坐标
        if(this->now_map.matrix[x_unit][y_unit]==1)//是路径
        {
            if(this->now_map.occupied_matrix[x_unit][y_unit]==0)//没有被占用
            {
                int direction=1;//1是右，2是左
                if(event->x()<first_x)
                {
                    direction=2;
                }
                pea* temp = new pea(x,y,direction);temp->direction=direction;
                if(direction==2)
                {
                    temp->image_path=":/ress/peaL.png";
                }
                this->now_map.occupied_matrix[x_unit][y_unit]=temp->ID;
                this->p_list.push_back(temp);
                food-=temp->value;
                update();
                this->tower.push_back(temp);
                temp->unit_x=x_unit;
                temp->unit_y=y_unit;
            }
            else
            {
                this->ui->textEdit->setText("该格子已被占用！");
            }
        }
        else
        {
            this->ui->textEdit->setText("该植物不可以种植在这里！");
        }
    }
    buliding_pea=0;
    first_x=0;first_y=0;
    if(this->mouse_has_plant&&event->x()<1200&&event->x()>250)
        //已经点击过一次植物

    {
        if(plant_type==1)//种植boluo
        {
            //qDebug()<<plant_type;
            if(food>=50)
            {
                newPlants(1,event->pos().y(),event->pos().x());
                update();
            }
            else
            {
                this->ui->textEdit->setText("您的阳光不足！");
            }
            this->mouse_has_plant=0;
        }
        if(plant_type==2)//种植忧郁蘑菇
        {
            if(food>=200)
            {
                newPlants(2,event->pos().y(),event->pos().x());
                update();
            }
            else
            {
                this->ui->textEdit->setText("您的阳光不足！");
            }
            this->mouse_has_plant=0;
        }
        if(plant_type==3)//种植闪电芦苇
        {
            if(food>=200)
            {
                newPlants(3,event->pos().y(),event->pos().x());
                update();
            }
            else
            {
                this->ui->textEdit->setText("您的阳光不足！");
            }
            this->mouse_has_plant=0;
        }
        if(plant_type==4)
        {
            if(food>=100)
            {
                this->buliding_pea=1;
                this->first_x=event->x();
                this->first_y=event->y();
            }
            else
            {
                this->ui->textEdit->setText("您的阳光不足！");
            }
            this->mouse_has_plant=0;
        }
    }

    if(this->removing&&event->x()<1200&&event->x()>250)
    {
        remove(event->pos().y(),event->pos().x());
        this->removing=0;
    }
}

void MainWindow::remove(int x, int y)
{
    int x_unit=(x-135)/75;
    int y_unit=(y-280)/75;
    x=x_unit*75+135;
    y=y_unit*75+280;
    if((this->now_map.matrix[x_unit][y_unit]==1)||this->now_map.tower_matrix[x_unit][y_unit]==1)//是路径
    {
        if(this->now_map.occupied_matrix[x_unit][y_unit]!=0)//被占用
        {
            int theID=find_Plant_byID(now_map.occupied_matrix[x_unit][y_unit]);
            this->p_list[theID]->if_alive=0;
            food += p_list[theID]->value*0.6;
            now_map.occupied_matrix[x_unit][y_unit]=0;
        }
    }
}
void MainWindow::newPlants(int type,int x,int y)//创建一个新的植物
 {
    if(type==0)//菜问
    {
        if(!this->now_map.list[0].path[prePlants].occupied)
            //当目前格子没有被占用时
        {
            x=( this->now_map.list[0].path[prePlants].x)*75+135;//??? //x y 是 4 8
            y=( this->now_map.list[0].path[prePlants].y)*75+280;
            this->now_map.list[0].path[prePlants].occupied=1;
            this->now_map.occupied_matrix[this->now_map.list[0].path[prePlants].x][this->now_map.list[0].path[prePlants].y]=1;
            prePlants++;//阶段一中的菜问专用
            Sett* temp = new Sett(x,y);
            this->p_list.push_back(temp);
            qDebug()<<"hereX:"<<x<<" "<<y;
        }
    }
    if(type==1)//菠萝
    {
        int x_unit=(x-135)/75;
        int y_unit=(y-280)/75;
        x=x_unit*75+135;
        y=y_unit*75+280;
        if(this->now_map.matrix[x_unit][y_unit]==1)//是路径
        {
            if(this->now_map.occupied_matrix[x_unit][y_unit]==0)//没有被占用
            {
                boluo* temp = new boluo(x,y);
                this->now_map.occupied_matrix[x_unit][y_unit]=temp->ID;
                this->p_list.push_back(temp);
                food-=temp->value;
                update();
                //创建boluo对象，并将它放到p_list中，之后就可以将他打印出来了
                temp->unit_x=x_unit;
                temp->unit_y=y_unit;
            }
            else
            {
                this->ui->textEdit->setText("该格子已被占用！");
            }
        }
        else
        {
            this->ui->textEdit->setText("该植物不可以种植在这里！");
        }

    }
    if(type==2)//忧郁蘑菇
    {
        int x_unit=(x-135)/75;
        int y_unit=(y-280)/75;
        x=x_unit*75+135-20;//竖直坐标
        y=y_unit*75+280-10;//横轴坐标
        if(this->now_map.tower_matrix[x_unit][y_unit]==1)//是防御塔路径
        {
            if(this->now_map.occupied_matrix[x_unit][y_unit]==0)//没有被占用
            {
                youyu* temp = new youyu(x,y);
                this->now_map.occupied_matrix[x_unit][y_unit]=temp->ID;
                this->p_list.push_back(temp);
                food-=temp->value;
                update();
                this->tower.push_back(temp);
                temp->unit_x=x_unit;
                temp->unit_y=y_unit;
            }
            else
            {
                this->ui->textEdit->setText("该格子已被占22用！");
            }
        }
        else
        {
            this->ui->textEdit->setText("该植物不可以种植在这里！");
        }
    }
    if(type==3)//闪电芦苇
    {
        int x_unit=(x-135)/75;
        int y_unit=(y-280)/75;
        x=x_unit*75+135-20;//竖直坐标
        y=y_unit*75+280-10;//横轴坐标
        if(this->now_map.tower_matrix[x_unit][y_unit]==1)//是防御塔路径
        {
            if(this->now_map.occupied_matrix[x_unit][y_unit]==0)//没有被占用
            {
                luwei* temp = new luwei(x,y);
                this->now_map.occupied_matrix[x_unit][y_unit]=temp->ID;
                this->p_list.push_back(temp);
                food-=temp->value;
                update();
                this->tower.push_back(temp);
                temp->unit_x=x_unit;
                temp->unit_y=y_unit;
            }
            else
            {
                this->ui->textEdit->setText("该格子已被占用！");
            }
        }
        else
        {
            this->ui->textEdit->setText("该植物不可以种植在这里！");
        }
    }
}

void MainWindow::paintEvent(QPaintEvent * )
{
    QPainter painter(this);
    //////////////画出地图包括出生点/////////////////
    painter.drawPixmap(-200,3,QPixmap(":/map2.jpg"));
    for(int i=0;i<(int)this->p_list.size();i++)
    {
        //qDebug()<<"当前植物数量："<<p_list.size();
        if(p_list[i]->if_alive)
        {
            if(p_list[i]->type == 1)//boluo
            {
                QPen pen(QColor(0,225,0));
                QBrush brush(QColor(0,200,0));
                painter.setBrush(brush);
                painter.setPen(pen);
                double rate=1.0*p_list[i]->life_value/p_list[i]->total_blood;
                painter.drawRect(p_list[i]->y+5,p_list[i]->x-12,rate*60,7);
                painter.drawPixmap(p_list[i]->y,p_list[i]->x,QPixmap(p_list[i]->image_path));
            }
            if(p_list[i]->type == 2)//youyu
            {
                QPen pen(QColor(0,0,255));
                QBrush brush(QColor(0,100,150));
                painter.setBrush(brush);
                painter.setPen(pen);
                double rate=1.0*p_list[i]->life_value/p_list[i]->total_blood;
                painter.drawRect(p_list[i]->y+15,p_list[i]->x+4,rate*60,7);
                painter.drawPixmap(p_list[i]->y,p_list[i]->x,QPixmap(p_list[i]->image_path));
            }
            if(p_list[i]->type == 3)//闪电芦苇
            {
                QPen pen(QColor(0,0,255));
                QBrush brush(QColor(0,100,150));
                painter.setBrush(brush);
                painter.setPen(pen);
                double rate=1.0*p_list[i]->life_value/p_list[i]->total_blood;
                painter.drawRect(p_list[i]->y+15,p_list[i]->x+4,rate*60,7);
                painter.drawPixmap(p_list[i]->y,p_list[i]->x,QPixmap(p_list[i]->image_path));
            }
            if(p_list[i]->type == 4)//豌豆射手
            {
                QPen pen(QColor(0,225,0));
                QBrush brush(QColor(0,200,0));
                painter.setBrush(brush);
                painter.setPen(pen);
                double rate=1.0*p_list[i]->life_value/p_list[i]->total_blood;
                painter.drawRect(p_list[i]->y+15,p_list[i]->x+4,rate*60,7);
                painter.drawPixmap(p_list[i]->y,p_list[i]->x,QPixmap(p_list[i]->image_path));
            }
        }
    }
    for(int i=0;i<(int)z_list.size();i++)
    {
        if(!z_list[i]->if_buried)//还没有被抬走
        {
            if(z_list[i]->type==1)
            {
                if(z_list[i]->if_alive)
                {
                    QPen pen(QColor(225,0,0));
                    QBrush brush(QColor(200,0,0));
                    painter.setBrush(brush);
                    painter.setPen(pen);
                    double rate=1.0*z_list[i]->life_value/z_list[i]->total_blood;
                    painter.drawRect(z_list[i]->y,z_list[i]->x-12,rate*55,7);
                }
                painter.drawPixmap(z_list[i]->y,z_list[i]->x,QPixmap(z_list[i]->image_path));
            }
            if(z_list[i]->type==2)
            {
                if(z_list[i]->if_alive)
                {
                    QPen pen(QColor(225,0,0));
                    painter.setPen(pen);
                    QBrush brush(QColor(200,0,0));
                    painter.setBrush(brush);
                    double rate=1.0*z_list[i]->life_value/z_list[i]->total_blood;
                    painter.drawRect(z_list[i]->y,z_list[i]->x-12,rate*65,7);

                }
                painter.drawPixmap(z_list[i]->y,z_list[i]->x,QPixmap(z_list[i]->image_path));
            }
            if(z_list[i]->type==3)
            {
                if(z_list[i]->if_alive)
                {
                    QPen pen(QColor(225,0,0));painter.setPen(pen);
                    QBrush brush(QColor(200,0,0));
                    painter.setBrush(brush);

                    double rate=1.0*z_list[i]->life_value/z_list[i]->total_blood;
                    painter.drawRect(z_list[i]->y,z_list[i]->x-12,rate*55,7);
                }
                painter.drawPixmap(z_list[i]->y,z_list[i]->x,QPixmap(z_list[i]->image_path));
            }
        }
    }
    for(int i=0;i<(int)tower.size();i++)
    {
        for(int j=0;j<(int)tower[i]->clip.size();j++)
        {
            if(tower[i]->clip[j].type==1)
            {
                if(tower[i]->clip[j].value>=0)//如果子弹存在就画出来
                {
                    tower[i]->clip[j].value-=1;
                    painter.drawPixmap(tower[i]->clip[j].ox,tower[i]->clip[j].oy,
                                   QPixmap(tower[i]->clip[j].image_path));
                    QPen pen(QColor(225,0,0));
                    painter.setPen(pen);
                    painter.drawRect(tower[i]->clip[j].vol);
                }
            }
            if(tower[i]->clip[j].type==3)//peas
            {
                if(tower[i]->clip[j].value>=0)
                {
                painter.drawPixmap(tower[i]->clip[j].ox+50,tower[i]->clip[j].oy+30,
                               QPixmap(tower[i]->clip[j].image_path));
                QPen pen(QColor(225,0,0));
                painter.setPen(pen);
                }
            }
            if(tower[i]->clip[j].type==2)
            {
                if(tower[i]->clip[j].value>=0)//如果子弹存在就画出来
                {
                    tower[i]->clip[j].value-=1;
                    QPen pen(QColor(0,100,100));
                    pen.setWidth(4);
                    painter.setPen(pen);
                    painter.drawLine(tower[i]->clip[j].x,tower[i]->clip[j].y,
                                   tower[i]->clip[j].ox,tower[i]->clip[j].oy);
                }
            }
        }
    }
}
void MainWindow::newzoombies(int type, int x, int y)
{
    this->clear_zms++;
    type=(QRandomGenerator::global()->bounded(100))%13;type++;
    //type=1;
    if(type<=8)
    {
        x=(this->paths[0].path[0].x)*75+135;//??? //x y 是 4 8
        y=(this->paths[0].path[0].y)*75+280;
        normal_zoombies* zm = new normal_zoombies(x,y);
        while(true)
        {
            int a=QRandomGenerator::global()->bounded(100)%((int)(this->paths.size()));
            if(!this->paths[a].ifair)//只要不是飞行路径就可以
            {
                zm->pathEnum=a;
                break;
            }
        }
        this->z_list.push_back(zm);
    }
    else if(type>=9&&type<=10)//气球僵尸，生成僵尸的时候，指定好僵尸的路径
    {
        x=(this->paths[0].path[0].x)*75+135;//??? //x y 是 4 8 //x是竖直
        y=(this->paths[0].path[0].y)*75+280;
        balloon_zoombies* zm = new balloon_zoombies(x,y);//
        zm->pathEnum=QRandomGenerator::global()->bounded(100)%2;
        this->z_list.push_back(zm);
    }
    else if(type>=11)
    {
        x=(this->paths[0].path[0].x)*75+135;//??? //x y 是 4 8
        y=(this->paths[0].path[0].y)*75+280;
        fire_zoombies* zm = new fire_zoombies(x,y);
        while(true)
        {
            int a=QRandomGenerator::global()->bounded(100)%((int)(this->paths.size()));
            if(!this->paths[a].ifair)//只要不是飞行路径就可以
            {
                zm->pathEnum=a;
                break;
            }
        }
        this->z_list.push_back(zm);
        this->long_zm.push_back(zm);
    }
}

void MainWindow::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId() == food_id)
    {
        QString str ="资源：";food +=5;
        str+=QString::number(food);
        ui->food_label->setText(str);
    }
    if(ev->timerId()==zoombies_newID)
    {
        int a=QRandomGenerator::global()->bounded(100)+1;
        //qDebug()<<a;
        if(a%2==0)
        {this->newzoombies();}
    }
    if(ev->timerId() == zoombies_timerID)
    {
        move(this->z_list);//调用move()函数，实现每个僵尸的移动
        update();
    }
    if(ev->timerId()==attack_id)
    {
        attack();
        update();
    }
    if(ev->timerId()==bullet_move_id)
    {
        for(int i=0;i<(int)tower.size();i++)
        {
            for(int j=0;j<(int)tower[i]->clip.size();j++)
            {
                if(tower[i]->clip[j].value>=0)//如果子弹存在就画出来
                {
                    tower[i]->clip[j].move();
                }
            }
        }
    }
}

void MainWindow::attack()
////这是一个单位行动的函数
{
    //陆地僵尸和植物的对决
    for(int j=0;j<(int)this->z_list.size();j++)
    {
        int i= z_list[j]->index_in_path;//当前僵尸所在的格数
        int qwe=z_list[j]->pathEnum;
        if(i!=(int)paths[qwe].path.size()-1)
        {
        if(this->now_map.occupied_matrix[paths[qwe].path[i+1].x][paths[qwe].path[i+1].y]!=0&&
                !this->z_list[j]->if_air)
            //当前僵尸的下一个格子存在植物
        {
            z_list[j]->if_struck=1;//僵尸被卡住不会移动
            int pIndex=find_Plant_byID(now_map.occupied_matrix[paths[qwe].path[i+1].x][paths[qwe].path[i+1].y]);
            if(z_list[j]->if_alive)
            {
                p_list[pIndex]->life_value-=z_list[j]->attack_value;
                if(z_list[j]->changing==0)
                {
                    z_list[j]->image_path=z_list[j]->eating_path1;
                    if(this->paths[qwe].path[i+1].y>this->paths[qwe].path[i].y)
                    {
                        z_list[j]->image_path=z_list[j]->eating_pathL;
                    }
                    z_list[j]->changing++;
                }
                else
                {
                    z_list[j]->image_path=z_list[j]->eating_path2;
                    if(this->paths[qwe].path[i+1].y>this->paths[qwe].path[i].y)
                    {
                        z_list[j]->image_path=z_list[j]->eating_pathL2;
                    }
                   z_list[j]->changing=0;
                }
                z_list[j]->life_value-=50;
                if(z_list[j]->life_value<=0)
                {
                    QString str="击杀了一个僵尸！ID:00";
                    food+=75;
                    qDebug()<<food;
                    str+=QString::number(z_list[j]->ID);
                    this->ui->textEdit->setText(str);
                    z_list[j]->if_alive=0;z_list[j]->life_value=0;
                }
                if(p_list[pIndex]->life_value<=0)
                {
                    now_map.occupied_matrix[paths[qwe].path[i+1].x][paths[qwe].path[i+1].y]=0;
                    z_list[j]->if_struck=0;
                    p_list[pIndex]->if_alive=0;z_list[j]->life_value=0;
                }
            }
        }
        else
        {
            z_list[j]->if_struck=0;
        }
        }
    }
    //防御塔植物对陆地僵尸的攻击
    for(int i=0;i<(int)this->tower.size();i++)
    {
        if(this->tower[i]->if_alive&&tower[i]->type==2)
        {
            //遍历僵尸数组，攻击每一个僵尸
            int num=0;
            for(int j=0;j<(int)this->z_list.size();j++)
            {
                //当矩形有交集且僵尸存活，则开始进行攻击
                if(this->z_list[j]->vol2.intersects(this->tower[i]->vol1)
                        &&z_list[j]->if_alive)
                {
                    duqi newone(z_list[j]->y,z_list[j]->x,100,tower[i]->y,tower[i]->x,":/duqi.png");
                    //tower[i]->clip.push_back(newone);
                    gas *bull=new gas(tower[i]->y+35,tower[i]->x+35,z_list[j]->y,z_list[j]->x);
                    tower[i]->clips.push_back(bull);
                    bull->move(this);
                    this->z_list[j]->life_value-=tower[i]->attack_value;num++;
                    if(z_list[j]->life_value<=0)
                    {
                        double judge=QRandomGenerator::global()->bounded(10)%2-0.5;
                        z_list[j]->y+=QRandomGenerator::global()->bounded(0,10)*judge*5;
                        QString str="击杀了一个僵尸！ID:00";
                        food+=50;
                        str+=QString::number(z_list[j]->ID);
                        this->ui->textEdit->setText(str);
                        z_list[j]->if_alive=0;z_list[j]->life_value=0;
                    }
                }
                //if(num>=3)break;
            }
        }
        if(this->tower[i]->if_alive&&tower[i]->type==3)//闪电芦苇的攻击
        {
            tower[i]->second++;
            if(tower[i]->second>=2)
            {
            //遍历僵尸数组，攻击每一个僵尸
            int num=0;
            for(int j=0;j<(int)this->z_list.size();j++)
            {
                //当矩形有交集且僵尸存活，则开始进行攻击
                if(this->z_list[j]->vol2.intersects(this->tower[i]->vol1)&&z_list[j]->if_alive)
                {
                    light newone(z_list[j]->y,z_list[j]->x,100,tower[i]->y+35,tower[i]->x+40);
                    int indexx=z_list[j]->index_in_path+1;
                    if(indexx>=(int)this->paths[this->z_list[j]->pathEnum].path.size()-1)
                    {
                        indexx=this->paths[this->z_list[j]->pathEnum].path.size()-1;
                    }
                    lighting *bull=new lighting(z_list[j]->y+35,z_list[j]->x+35,
                                                this->paths[this->z_list[j]->pathEnum].path[indexx].y*75-280,
                                                this->paths[this->z_list[j]->pathEnum].path[indexx].x*75-135);
                    //tower[i]->clips.push_back(bull);
                    bull->move(this);
                    tower[i]->clip.push_back(newone);
                    this->z_list[j]->life_value-=tower[i]->attack_value;num++;
                    if(z_list[j]->life_value<=0)
                    {
                        double judge=QRandomGenerator::global()->bounded(10)%2-0.5;
                        z_list[j]->y+=QRandomGenerator::global()->bounded(0,10)*judge*5;
                        QString str="击杀了一个僵尸！ID:00";
                        food+=50;
                        str+=QString::number(z_list[j]->ID);
                        this->ui->textEdit->setText(str);
                        z_list[j]->if_alive=0;
                        z_list[j]->life_value=0;
                    }
                }
                if(num>=1)break;
            }tower[i]->second=0;
            }
        }
        if(this->tower[i]->if_alive&&tower[i]->type==4)//豌豆射手的攻击
        {
            tower[i]->second++;
            if(tower[i]->second>=5)
            {
            tower[i]->second=0;
            for(int j=0;j<(int)this->z_list.size();j++)
            {
                if(this->z_list[j]->vol2.intersects(this->tower[i]->vol1)
                        &&z_list[j]->if_alive&&!z_list[j]->if_air)
                {
                    peas newone(z_list[j]->y,z_list[j]->x,100,tower[i]->y,
                                tower[i]->x,this->tower[i]->direction,tower[i]->value);
                    tower[i]->clip.push_back(newone);
                    break;//攻击一个即可
                }
            }
            }
        }
    }
    for(int i=0;i<(int)long_zm.size();i++)
    {
        if(long_zm[i]->type==3&&long_zm[i]->if_alive)
        {
            for(int j=0;j<(int)p_list.size();j++)
            {
                if(long_zm[i]->damage_rect.intersects(p_list[j]->vol)&&p_list[j]->if_alive)
                {
                    p_list[j]->life_value-=long_zm[i]->attack_value;
                    if(p_list[j]->life_value<=0)
                    {
                        now_map.occupied_matrix[p_list[j]->unit_x][p_list[j]->unit_y]=0;
                        p_list[j]->if_alive=0;
                        if(z_list[j]->life_value<=0)
                        {
                            double judge=QRandomGenerator::global()->bounded(10)%2-0.5;
                            z_list[j]->y+=QRandomGenerator::global()->bounded(0,10)*judge*5;
                            QString str="击杀了一个僵尸！ID:00";
                            food+=50;
                            str+=QString::number(z_list[j]->ID);
                            this->ui->textEdit->setText(str);
                            z_list[j]->if_alive=0;
                        }
                    }
                }
            }
        }
    }
}
void MainWindow::move(vector<zoombies*>z_list)
//按照路径实现敌人坐标的变化，之后再调用update()函数打印出僵尸的位置
//传入僵尸列表，对每个存活的僵尸做出坐标变化
{
    if(this->clear_zms==0)
    {
        this->z_list.clear();
    }
    for(int j=0;j<(int)z_list.size();j++)
    {
        int i= z_list[j]->index_in_path;//当前僵尸所在的格数
        int qwe=z_list[j]->pathEnum;
        //该僵尸要走的路径
        if(i>=(int)this->paths[qwe].path.size()-1)//总共十八格，当僵尸i等于17时候已经走到终点了
        {
            if(z_list[j]->if_alive)
            {
                blood-=1;
                QString str="血量：";
                str+=QString::number(blood);
                this->ui->blood_label->setText(str);
                if(blood<=0)
                {
                    this->goingon=0;
                    ui->textEdit->setText("游戏失败！");
                    for(int i=0;i<(int)timer_list.size();i++)
                    {
                        timer_list[i]->stop();
                    }
                    ui->pause_btn->setText("继续游戏");
                    goingon=0;
                }
            }
            z_list[j]->if_alive=0;
            z_list[j]->if_buried=1;
            continue;
        }
        if(!z_list[j]->if_alive&&!z_list[j]->if_buried)
        {
            z_list[j]->image_path=z_list[j]->dying_path;
            z_list[j]->life_value-=4;
            if(z_list[j]->life_value<=-75)
            {
                z_list[j]->dying_path=z_list[j]->bury_path;
            }
            if(z_list[j]->life_value<=-225)
            {
                z_list[j]->if_buried=1;
                this->clear_zms--;
            }
        }
        if(z_list[j]->if_alive&&!z_list[j]->if_struck)//当僵尸存活的时候进行移动
        {
            if(this->paths[qwe].path[i+1].x>this->paths[qwe].path[i].x)//应该向下走
            {
                z_list[j]->x+=1;
                z_list[j]->dying_path=z_list[j]->dying_path1;
                z_list[j]->bury_path=z_list[j]->bury_path1;
                if(z_list[j]->x==(this->paths[qwe].path[i+1].x*75)+135)
                {
                    z_list[j]->index_in_path++;
                }
                //接下来在进行脚步转换
                z_list[j]->changing+=3;
                if((z_list[j]->changing>=0)&&z_list[j]->changing<40)//0-100使用path
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                }
                else if(z_list[j]->changing>=40&&z_list[j]->changing<80)//100-200使用nor_path
                {
                    z_list[j]->image_path=z_list[j]->nor_path2;
                }
                else if(z_list[j]->changing>=80)
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                    z_list[j]->changing=0;
                }
            }
            if(this->paths[qwe].path[i+1].x<this->paths[qwe].path[i].x)//应该向上走
            {
                z_list[j]->dying_path=z_list[j]->dying_path1;
                z_list[j]->bury_path=z_list[j]->bury_path1;
                //接下来在进行脚步转换
                z_list[j]->changing+=3;
                if((z_list[j]->changing>=0)&&z_list[j]->changing<40)//0-100使用path
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                }
                else if(z_list[j]->changing>=40&&z_list[j]->changing<80)//100-200使用nor_path
                {
                    z_list[j]->image_path=z_list[j]->nor_path2;
                }
                else if(z_list[j]->changing>=80)
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                    z_list[j]->changing=0;
                }
                z_list[j]->x-=1;
                if(z_list[j]->x==(this->paths[qwe].path[i+1].x*75)+135)
                {
                    z_list[j]->index_in_path++;
                }
            }
            if(this->paths[qwe].path[i+1].y>this->paths[qwe].path[i].y)//应该向右走
            {
                z_list[j]->dying_path=z_list[j]->dying_forL;
                z_list[j]->bury_path=z_list[j]->bury_forL;
                //接下来在进行脚步转换
                z_list[j]->changing+=3;
                if((z_list[j]->changing>=0)&&z_list[j]->changing<40)//0-100使用path
                {
                    z_list[j]->image_path=z_list[j]->nor_path_forL;
                }
                else if(z_list[j]->changing>=40&&z_list[j]->changing<80)//100-200使用nor_path
                {
                    z_list[j]->image_path=z_list[j]->nor_path_forL2;
                }
                else if(z_list[j]->changing>=80)
                {
                    z_list[j]->image_path=z_list[j]->nor_path_forL2;
                    z_list[j]->changing=0;
                }
                z_list[j]->y+=1;
                if(z_list[j]->y==(this->paths[qwe].path[i+1].y*75)+280)
                {
                    z_list[j]->index_in_path++;
                }
            }
            if(this->paths[qwe].path[i+1].y<this->paths[qwe].path[i].y)//应该向左走
            {
                z_list[j]->dying_path=z_list[j]->dying_path1;
                z_list[j]->bury_path=z_list[j]->bury_path1;
                //接下来在进行脚步转换
                z_list[j]->changing+=3;
                if((z_list[j]->changing>=0)&&z_list[j]->changing<40)//0-100使用path
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                }
                else if(z_list[j]->changing>=40&&z_list[j]->changing<80)//100-200使用nor_path
                {
                    z_list[j]->image_path=z_list[j]->nor_path2;
                }
                else if(z_list[j]->changing>=80)
                {
                    z_list[j]->image_path=z_list[j]->nor_path;
                    z_list[j]->changing=0;
                }
                z_list[j]->y-=1;
                if(z_list[j]->y==(this->paths[qwe].path[i+1].y*75)+280)
                {
                    z_list[j]->index_in_path++;
                }
            }
        }
        z_list[j]->vol2=QRect(z_list[j]->y,z_list[j]->x,80,80);
        if(z_list[j]->type==3)
        {
            z_list[j]->damage_rect=QRect(z_list[j]->y-65,z_list[j]->x-65,230,230);
        }
    }
    for(int i=0;i<(int)tower.size();i++)
    {
        if(tower[i]->type==4)//豌豆射手
        {
            for(int j=0;j<(int)tower[i]->clip.size();j++)
            {
                tower[i]->clip[j].move();
                tower[i]->clip[j].vol=QRect(tower[i]->clip[j].ox-10,tower[i]->clip[j].oy+30,
                                            50,50);
                if(tower[i]->clip[j].ox>=tower[i]->clip[j].x)
                {
                    tower[i]->clip[j].value=-1;
                }
                for(int h=0;h<(int)z_list.size();h++)
                {
                    if(tower[i]->clip[j].vol.intersects(z_list[h]->vol2)
                            &&!z_list[h]->if_air&&z_list[h]->if_alive&&tower[i]->clip[j].value>=0)
                    {
                        tower[i]->clip[j].value=-1;
                        z_list[h]->life_value-=tower[i]->clip[j].atk;
                        if(z_list[h]->life_value<=0)
                        {
                            z_list[h]->if_alive=0;
                            z_list[h]->life_value=0;
                        }
                    }
                }
            }
        }
    }
}
int MainWindow::find_Plant_byID(int id)
{
    int index=0;
    for(;index<(int)this->p_list.size();index++)
    {
        if(this->p_list[index]->ID==id)
        {
            return index;
        }
    }
    return 0;
}
int MainWindow::find_zoombies_byID(int id)
{
    int index=0;
    for(;index<(int)this->z_list.size();index++)
    {
        if(this->z_list[index]->ID==id)
        {
            return index;
        }
    }
    return 0;
}
MainWindow::~MainWindow()
{
    delete ui;
}
