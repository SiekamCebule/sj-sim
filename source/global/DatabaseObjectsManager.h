#ifndef DATABASEOBJECTSMANAGER_H
#define DATABASEOBJECTSMANAGER_H

#include <QMap>
#include "../utilities/ClassWithID.h"

class DatabaseObjectsManager
{
public:
    DatabaseObjectsManager();

private:
    QMap<ulong, ClassWithID *> objects;

public:
    void addObject(ClassWithID * object);
    void removeObject(ulong ID);
    ClassWithID * getObjectByID(ulong ID);
    void clear();

    template<typename T>
    void fill(QVector<T> * vector)
    {
        for(auto & o : *vector)
            addObject(dynamic_cast<ClassWithID *>(&o));
    }
    template<typename T>
    void fill(QVector<T *> * vector)
    {
        for(auto & o : *vector)
            addObject(dynamic_cast<ClassWithID *>(o));
    }
    QMap<ulong, ClassWithID *> getObjects() const;
};

#endif // DATABASEOBJECTSMANAGER_H
