#include "mylabel.h"
#include <QMouseEvent>

mylabel::mylabel(QWidget *parent) : QWidget(parent)
{

}

void mylabel::mousePressEvent(QMouseEvent *ev)
{
    int unit_x = ev->pos().x();
    int unit_y = ev->pos().y();

}
