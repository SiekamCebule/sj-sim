#include "ClassWithID.h"

extern IDGenerator globalIDGenerator;


ClassWithID::ClassWithID()
{
    ID = globalIDGenerator.generateNewID();
}

ulong ClassWithID::regenerateID()
{
    if(globalIDGenerator.hasValue(ID)){
        resetID();
    }
    ID = globalIDGenerator.generateNewID();
    return ID;
}

void ClassWithID::resetID()
{
    globalIDGenerator.removeID(ID);
    ID = 0;
}

ulong ClassWithID::getID() const
{
    return ID;
}

void ClassWithID::setID(ulong id)
{
    if(globalIDGenerator.hasValue(id))
        globalIDGenerator.removeID(id);

    globalIDGenerator.addExistingID(id);
    setID(id);
}
