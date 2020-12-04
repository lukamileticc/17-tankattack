#ifndef INPUT_HPP
#define INPUT_HPP

#include <QKeyEvent>
#include <QGraphicsItem>
#include <QTimer>
//#include "code/include/Tank.hpp"

#define key_up  1
#define key_down  2
#define key_right  4
#define key_left  8
#define key_rocket_launch 16


class Input : public QGraphicsItem{
public:
    Input();
    virtual ~Input() = default;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QTimer *timer;

//protected:
    unsigned int key_tank1;
    unsigned int key_tank2;
    bool k_w,k_s,k_d,k_a,k_space;
    bool k_up,k_down,k_right,k_left,k_enter;
    bool pause;
    bool escape;


};


#endif // INPUT_HPP
