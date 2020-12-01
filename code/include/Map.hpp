#ifndef MAP_HPP
#define MAP_HPP
#include <vector>
#include "Wall.hpp"
#include <QGraphicsScene>

#include <fstream>
#include <string>
#include <sstream>
class Map {
public:

    Map() = default;
    Map(const char * file);
    Map(int mNumOfWalls, const std::vector<Wall*> &mWalls);
    ~Map()  = default;
    int getNumOfWalls() const;

    void setNumOfWalls(int mNumOfWalls);

    const std::vector<Wall*> getWalls() const;

    void setWalls(const std::vector<Wall*> &mWalls);


private:
    int m_num_of_walls;
    std::vector<Wall*> m_walls;

};


#endif //MAP_HPP
