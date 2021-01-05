#include "code/include/Input.hpp"
#include <QTimer>
#include <QPainter>
#include <iostream>
#include <QDebug>

Input::Input(){
    timer = new QTimer();
    k_w = false,k_s = false, k_a = false,k_d = false,k_space = false;
    k_up = false, k_down = false; k_right = false, k_left = false, k_enter = false;
    pause = false;
    escape = false;
}
Input::~Input()
{
    delete timer;
}

void Input::keyPressEvent(QKeyEvent *event){

    if (event->isAutoRepeat()) {
                return;
    }
    switch(event->key()){
        case Qt::Key_W:
            k_w = true;
            break;
        case Qt::Key_S:
            k_s = true;
            break;
        case Qt::Key_D:
            k_d = true;
            break;
        case Qt::Key_A:
            k_a = true;
            break;
        case Qt::Key_Space:
            k_space = true;
            break;
        case Qt::Key_Up:
            k_up = true;
            break;
        case Qt::Key_Down:
            k_down = true;
            break;
        case Qt::Key_Right:
            k_right = true;
            break;
        case Qt::Key_Left:
            k_left = true;
            break;
        case Qt::Key_Enter:
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
}

void Input::keyReleaseEvent(QKeyEvent *event){

    if (event->isAutoRepeat()) {
            return;
    }

    switch(event->key()){
        case Qt::Key_W:
            k_w = false;
            break;
        case Qt::Key_S:
            k_s = false;
            break;
        case Qt::Key_D:
            k_d = false;
            break;
        case Qt::Key_A:
            k_a = false;
            break;
        case Qt::Key_Space:
            k_space = false;
            break;
        case Qt::Key_Up:
            k_up = false;
            break;
        case Qt::Key_Down:
            k_down = false;
            break;
        case Qt::Key_Right:
            k_right = false;
            break;
        case Qt::Key_Left:
            k_left = false;
            break;
        case Qt::Key_Enter:
            k_enter = false;
            break;
        default:
            break;
    }
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
