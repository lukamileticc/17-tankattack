#include "code/include/button.hpp"
#include <QBrush>

Button::Button(QString text, int height, int width, QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
{

    this->setRect(0,0,width,height);
    QBrush brush;
    brush.setTexture(QPixmap(":/resources/images/background.jpg"));
    this->setBrush(brush);

    m_text = new QGraphicsTextItem(text,this);
    //postavljamo poziciju m_texta tako da se nalazi u button-u
    m_text->setPos(this->rect().width()/2 - m_text->boundingRect().width()/2,
                   this->rect().height()/2 - m_text->boundingRect().height()/2
                   );

    m_text->setDefaultTextColor(Qt::white);


}
Button::~Button()
{
    delete m_text;
}
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    //kada se klikne na button -- on treba da emituje signal
    //emit this->button_was_clicked();
}
