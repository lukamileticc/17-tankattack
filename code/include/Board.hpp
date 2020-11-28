#ifndef BOARD_HPP
#define BOARD_HPP
#include <QPainter>
#include <memory>
#include <utility>
#include <vector>
#include "code/include/Map.hpp"
#include "code/include/Tank.hpp"


class Board
{
public:
    Board(std::shared_ptr<const Map> map, Tank player1, Tank player2, std::vector<QImage> images);

    ~Board() = default;

    void draw(QPainter* painter);

    int get_board_width() const;
    int get_board_height() const;

    //void action_performed (action_event);

protected:
    bool m_end_of_round = false;
    bool m_end_of_game = false;

private:
    const int m_board_width = 1500;
    const int m_board_height = 1200;

    std::vector<QImage> m_images; 
    /*
    Mozda bude potrebno ubacivanje nekih slicica

    std::vector<QPixmap> scaled_pixmaps_;
    QPixmap map_scaled_pixmap_;

    */
    Tank m_player1; // include "Tank.hpp"
    Tank m_player2;

    std::shared_ptr<const Map> map; // include "Map.hpp"

    unsigned m_result_p1 = 0; // result for player1 
    unsigned m_result_p2 = 0; // result for player2

};


#endif
