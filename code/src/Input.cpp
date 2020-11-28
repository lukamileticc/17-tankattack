#include "code/include/Input.hpp"
#include <QTimer>
#include <iostream>

Input::Input(){
    timer = new QTimer();
    key_tank1 = 0;
    key_tank2 = 0;
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
            break;
        case Qt::Key_S:
            key_tank1 |= key_down;
            break;
        case Qt::Key_D:
            key_tank1 |= key_right;
            break;
        case Qt::Key_A:
            key_tank1 |= key_left;
            break;
        case Qt::Key_Space:
            key_tank1 |= key_rocket_launch;
            break;
        case Qt::Key_Up:
            key_tank2 |= key_up;
            break;
        case Qt::Key_Down:
            key_tank2 |= key_down;
            break;
        case Qt::Key_Right:
            key_tank2 |= key_right;
            break;
        case Qt::Key_Left:
            key_tank2 |= key_left;
            break;
        case Qt::Key_Enter:
            key_tank2 |= key_rocket_launch;
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
    std::cout << "zavrsen key press event metod" << std::endl;
}

void Input::keyReleaseEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
            return;
    }
    switch(event->key()){
        case Qt::Key_W:
            key_tank1 &= ~key_up;
            break;
        case Qt::Key_S:
            key_tank1 &= ~key_down;
            break;
        case Qt::Key_D:
            key_tank1 &= ~key_right;
            break;
        case Qt::Key_A:
            key_tank1 &= ~key_left;
            break;
        case Qt::Key_Space:
            key_tank1 &= ~key_rocket_launch;
            break;
        case Qt::Key_Up:
            key_tank2 &= ~key_up;
            break;
        case Qt::Key_Down:
            key_tank2 &= ~key_down;
            break;
        case Qt::Key_Right:
            key_tank2 &= ~key_right;
            break;
        case Qt::Key_Left:
            key_tank2 &= ~key_left;
            break;
        case Qt::Key_Enter:
            key_tank2 &= ~key_rocket_launch;
            break;
        default:
            break;
    }
    std::cout << "zavrsen key event release metod" << std::endl;
}
