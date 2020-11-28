#ifndef INPUT_HPP
#define INPUT_HPP

#include <QKeyEvent>
#include <QGraphicsItem>

#define key_up  1
#define key_down  2
#define key_right  4
#define key_left  8
#define key_rocket_launch 16


class Input : public QGraphicsItem{
public:
    Input();
    ~Input();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    unsigned int key_tank1;
    unsigned int key_tank2;
    bool pause;
    bool escape;
    bool rocket_launch;
    QTimer *timer;


};


#endif // INPUT_HPP
