#include "IDGenerator.h"

IDGenerator globalIDGenerator;

IDGenerator::IDGenerator() : lastID(0)
{
}

bool IDGenerator::hasValue(ulong value)
{
    return values.contains(value);
}

ulong IDGenerator::generateNewID()
{
    int i=1;
    while(values.contains(i))
    {
        i++;
    }
    values.insert(i);
    lastID = i;
    return i;
}

bool IDGenerator::removeID(ulong ID)
{
    if(!values.contains(ID))
        return false;
    else
    {
        values.remove(ID);
        return true;
    }
}

void IDGenerator::addExistingID(ulong ID)
{
    if(hasValue(ID) == false)
        values.insert(ID);
}

ulong IDGenerator::getLastID() const
{
    return lastID;
}
