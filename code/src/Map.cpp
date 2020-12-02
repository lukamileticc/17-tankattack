#include "../include/Map.hpp"


Map::Map(int mNumOfWalls, const std::vector<Wall*> &mWalls) : m_num_of_walls(mNumOfWalls), m_walls(mWalls) {}

Map::Map(const char * file)
{
    QFile inputFile(file);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       std::vector<int>coordinates;
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList lista=line.split(" ");
          for (auto broj : lista)
              coordinates.push_back(broj.toInt());
          Wall *w1 = new Wall(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
          m_walls.push_back(w1);
          coordinates.clear();
       }
    }
    inputFile.close();
    Map::setNumOfWalls(m_walls.size());

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

