#include "../include/Map.hpp"
#include <iostream>
Map::Map(int mNumOfWalls, const std::vector<Wall*> &mWalls) : m_num_of_walls(mNumOfWalls), m_walls(mWalls) {}

Map::Map(std::string file)
{

    std::string line;
    std::ifstream f1;

    f1.open(file);

    if(f1.is_open()){
        std::string segment;
        std::vector<int>coordinates;
        while(std::getline(f1, line))
        {
            std::cout << line;
            std::stringstream test(line);
            while(std::getline(test, segment, ' '))
            {
                coordinates.push_back(std::stoi(segment));
            }
            Wall *w1 = new Wall(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
            std::cout << coordinates[0] << coordinates[1] << coordinates[2] << coordinates[3];
            m_walls.push_back(w1);
            coordinates.clear();
        }
    }
    Map::setNumOfWalls(m_walls.size());

    f1.close();
}


int Map::getNumOfWalls() const {
    return m_num_of_walls;
}

void Map::setNumOfWalls(int mNumOfWalls) {
    m_num_of_walls = mNumOfWalls;
}

const std::vector<Wall*> Map::getWalls() const {
    return m_walls;
}

void Map::setWalls(const std::vector<Wall*> &mWalls) {
    m_walls = mWalls;
}

