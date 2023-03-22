#include "JumpManipulator.h"

JumpManipulator::JumpManipulator()
{
    isActive = false;
}

bool JumpManipulator::getIsActive() const
{
    return isActive;
}

void JumpManipulator::setIsActive(bool newIsActive)
{
    isActive = newIsActive;
}
