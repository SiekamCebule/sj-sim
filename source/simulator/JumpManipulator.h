#ifndef JUMPMANIPULATOR_H
#define JUMPMANIPULATOR_H


class JumpManipulator
{
public:
    JumpManipulator();

private:
    bool isActive;

public:
    bool getIsActive() const;
    void setIsActive(bool newIsActive);
};

#endif // JUMPMANIPULATOR_H
