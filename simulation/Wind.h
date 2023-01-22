#ifndef WIND_H
#define WIND_H


class Wind
{
public:

    Wind(short direction = Null, double value = 0);
    enum Direction{
        Null,
        Front,
        Back,
        Left,
        Right
    };

private:
    short direction;
    double value;

public:
    short getDirection() const;
    void setDirection(short newDirection);
    double getValue() const;
    void setValue(double newValue);
};

#endif // WIND_H
