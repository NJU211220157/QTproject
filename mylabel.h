#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>

class mylabel : public QWidget
{
    Q_OBJECT
public:
    explicit mylabel(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);

signals:

};

#endif // MYLABEL_H
