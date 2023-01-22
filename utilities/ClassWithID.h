#ifndef CLASSWITHID_H
#define CLASSWITHID_H

#include "IDGenerator.h"

class ClassWithID
{
public:
    ClassWithID();

    ulong regenerateID();
    void resetID();
    ulong getID() const;

protected:
    ulong ID;
};

#endif // CLASSWITHID_H
