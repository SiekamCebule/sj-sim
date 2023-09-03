#include "IDGenerator.h"

#include <QDebug>

IDGenerator globalIDGenerator;

IDGenerator::IDGenerator() : lastID(0)
{
    freezed = false;
}

bool IDGenerator::hasValue(ulong value)
{
    return values.contains(value);
}

ulong IDGenerator::generateNewID()
{
    if(freezed == false){
        int i=1;
        while(values.contains(i))
        {
            i++;
        }
        values.insert(i);
        lastID = i;
        return i;
    }
    return 0;
}

bool IDGenerator::removeID(ulong ID)
{
    if(freezed == false){
        if(!values.contains(ID))
            return false;
        else
        {
            values.remove(ID);
            return true;
        }
    }
    return false;
}

void IDGenerator::addExistingID(ulong ID)
{
    if(freezed == false){
        if(hasValue(ID) == false)
            values.insert(ID);
    }
}

ulong IDGenerator::getLastID() const
{
    return lastID;
}

QSet<ulong> IDGenerator::getValues() const
{
    return values;
}

void IDGenerator::reset()
{
    values.clear();
}

bool IDGenerator::getFreezed() const
{
    return freezed;
}

void IDGenerator::setFreezed(bool newFreezed)
{
    freezed = newFreezed;
}
