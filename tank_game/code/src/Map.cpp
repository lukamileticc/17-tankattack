#include "../include/Map.hpp"

Map::Map(int mNumOfWalls, const std::vector<Wall> &mWalls) : m_num_of_walls(mNumOfWalls), m_walls(mWalls) {}

int Map::getNumOfWalls() const {
    return m_num_of_walls;
}

void Map::setNumOfWalls(int mNumOfWalls) {
    m_num_of_walls = mNumOfWalls;
}

const std::vector<Wall> &Map::getWalls() const {
    return m_walls;
}

void Map::setWalls(const std::vector<Wall> &mWalls) {
    m_walls = mWalls;
}
