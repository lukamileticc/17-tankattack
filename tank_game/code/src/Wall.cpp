#include "../include/Wall.hpp"


Wall::Wall(int mX, int mY, int mLength, int mWeight) : m_x(mX), m_y(mY), m_length(mLength), m_weight(mWeight) {}

int Wall::getX() const {
    return m_x;
}

void Wall::setX(int mX) {
    m_x = mX;
}

int Wall::getY() const {
    return m_y;
}

void Wall::setY(int mY) {
    m_y = mY;
}

int Wall::getLength() const {
    return m_length;
}

void Wall::setLength(int mLength) {
    m_length = mLength;
}

int Wall::getWeight() const {
    return m_weight;
}

void Wall::setWeight(int mWeight) {
    m_weight = mWeight;
}
