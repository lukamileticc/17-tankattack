#include "code/include/Input.hpp"
#include <QTimer>
#include <QPainter>
#include <iostream>
#include <QDebug>

Input::Input(){
    timer = new QTimer();
    key_tank1 = 0;
    key_tank2 = 0;
    k_w = false,k_s = false, k_a = false,k_d = false,k_space = false;
    k_up = false, k_down = false; k_right = false, k_left = false, k_enter = false;
    pause = false;
    escape = false;
}

void Input::keyPressEvent(QKeyEvent *event){

    if (event->isAutoRepeat()) {
                return;
    }
    switch(event->key()){
        case Qt::Key_W:
            key_tank1 |= key_up;
            k_w = true;
            break;
        case Qt::Key_S:
            key_tank1 |= key_down;
            k_s = true;
            break;
        case Qt::Key_D:
            key_tank1 |= key_right;
            k_d = true;
            break;
        case Qt::Key_A:
            key_tank1 |= key_left;
            k_a = true;
            break;
        case Qt::Key_Space:
            key_tank1 |= key_rocket_launch;
            k_space = true;
            break;
        case Qt::Key_Up:
            key_tank2 |= key_up;
            k_up = true;
            break;
        case Qt::Key_Down:
            key_tank2 |= key_down;
            k_down = true;
            break;
        case Qt::Key_Right:
            key_tank2 |= key_right;
            k_right = true;
            break;
        case Qt::Key_Left:
            key_tank2 |= key_left;
            k_left = true;
            break;
        case Qt::Key_Enter:
            key_tank2 |= key_rocket_launch;
            qDebug() << "dsadsa";
            k_enter = true;
            break;
        case Qt::Key_P:
            pause = !pause;
            break;
        case Qt::Key_Escape:
            escape = !escape;
            break;
        default:
            break;
    }
//    std::cout << "zavrsen key press event metod" << std::endl;
}

void Input::keyReleaseEvent(QKeyEvent *event){

    if (event->isAutoRepeat()) {
            return;
    }

    switch(event->key()){
        case Qt::Key_W:
            key_tank1 &= ~key_up;
            k_w = false;
            break;
        case Qt::Key_S:
            key_tank1 &= ~key_down;
            k_s = false;
            break;
        case Qt::Key_D:
            key_tank1 &= ~key_right;
            k_d = false;
            break;
        case Qt::Key_A:
            key_tank1 &= ~key_left;
            k_a = false;
            break;
        case Qt::Key_Space:
            key_tank1 &= ~key_rocket_launch;
            k_space = false;
            break;
        case Qt::Key_Up:
            key_tank2 &= ~key_up;
            k_up = false;
            break;
        case Qt::Key_Down:
            key_tank2 &= ~key_down;
            k_down = false;
            break;
        case Qt::Key_Right:
            key_tank2 &= ~key_right;
            k_right = false;
            break;
        case Qt::Key_Left:
            key_tank2 &= ~key_left;
            k_left = false;
            break;
        case Qt::Key_Enter:
            key_tank2 &= ~key_rocket_launch;
            k_enter = false;
            break;
        default:
            break;
    }
//    std::cout << "zavrsen key event release metod" << std::endl;
}

void Input::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Input::boundingRect() const
{
    return QRectF(0, 0, 1, 1);
}
