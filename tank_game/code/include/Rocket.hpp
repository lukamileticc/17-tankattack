#ifndef ROCKET_HPP
#define ROCKET_HPP

#include <QPainter>
#include <memory>
#include <utility>
#include "Map.hpp"
#include "Tank.hpp"

class Rocket : public QGraphicsItem {  // mozda treba implementirati klasu Movable, koju cemo nasledjivati za sve funkcije
public:

//  buduci da klasu RocketParameters nismo kreirali, koristicemo obican int sve dok se ne saglasimo sta ta klasa treba da predstavlja
//  Rocket(std::shared_ptr<const Map> map, std::shared_ptr<Tank> tank, RocketParameters parameters);

    Rocket(std::shared_ptr<const Map> map, std::shared_ptr<Tank> tank, int parameters);
    ~Rocket() = default;

    void draw(QPainter *painter);
    QRectF boundingRect() const;

    int get_x() const;
    int get_y() const; // napraviti strukturu koordinate

    int get_power() const;
    int get_speed() const;
    bool get_break_obstacle() const;

    int get_board_width() const;
    int get_board_height() const;

private:
    int m_x;
    int m_y;

    std::shared_ptr<const Map> m_map;
    const std::shared_ptr<const Tank> m_tank;

    int m_parameters;
    int m_speed;
    int m_power;
    bool m_break_obstacle; // ako budemo implementirali da mozemo da lomimo zidove

    const int m_board_width = 1500;
    const int m_board_height = 1200;
};

#endif //ROCKET_HPP


