#include "functions.h"
#include <math.h>

double roundDoubleToHalf(double number)
{
    return (std::floor((number * 2) + 0.5) / 2);
}
