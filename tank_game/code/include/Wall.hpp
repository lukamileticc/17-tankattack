#ifndef WALL_HPP
#define WALL_HPP



class Wall {

public:
    Wall(int mX, int mY, int mLength, int mWeight);

    ~Wall()=default;

    int getX() const;

    void setX(int mX);

    int getY() const;

    void setY(int mY);

    int getLength() const;

    void setLength(int mLength);

    int getWeight() const;

    void setWeight(int mWeight);

private:
    int m_x;
    int m_y;
    int m_length;
    int m_weight;

};


#endif //WALL_HPP
