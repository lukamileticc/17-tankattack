#ifndef INPUT_HPP
#define INPUT_HPP

#include <QKeyEvent>
#include <QGraphicsItem>
#include <QTimer>

enum class Rocket_type{
    Low_power,
    Medium_power,
};

class Input : public QGraphicsItem {
public:
    Input();
    ~Input();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QTimer *timer = nullptr;

    bool k_w,k_s,k_d,k_a,k_space;
    bool k_up,k_down,k_right,k_left,k_enter;
    bool pause;
    bool escape;
};

#endif // INPUT_HPP
