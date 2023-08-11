#ifndef CLASSWITHID_H
#define CLASSWITHID_H

#include "../global/IDGenerator.h"

class ClassWithID
{
public:
    ClassWithID();
    ~ClassWithID();

    bool operator==(const ClassWithID & r) const;

    void regenerateID();
    void generateID(); //bez usuwania z generatora starego ID

    void resetID();
    ulong getID() const;
    void setID(ulong id);

protected:
    ulong ID;
};

#endif // CLASSWITHID_H
