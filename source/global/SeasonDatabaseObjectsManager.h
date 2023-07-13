#ifndef SEASONDATABASEOBJECTSMANAGER_H
#define SEASONDATABASEOBJECTSMANAGER_H

#include <QMap>
#include "../utilities/ClassWithID.h"

class SeasonDatabaseObjectsManager
{
public:
    SeasonDatabaseObjectsManager();

private:
    QMap<ulong, ClassWithID *> objects;

public:
    void addObject(ClassWithID * object);
    void removeObject(ulong ID);
    ClassWithID * getObjectByID(ulong ID);
    void clear();
};

#endif // SEASONDATABASEOBJECTSMANAGER_H
