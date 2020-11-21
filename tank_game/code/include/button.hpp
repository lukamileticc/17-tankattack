#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QString>
#include <QGraphicsTextItem>


//posto button mora da emituje signal neophodno je da nasledi klasu QObject!
class Button : public QGraphicsRectItem{

public:
    Button(QString text, int height, int width, QGraphicsItem *parent = nullptr);
    ~Button();

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

//signals; kasnije cemo odraditi
    void button_was_clicked();

private:
    QGraphicsTextItem *m_text;
};

#endif // BUTTON_HPP
