#include "DatabaseObjectsManager.h"

DatabaseObjectsManager::DatabaseObjectsManager()
{

}

QMap<ulong, ClassWithID *> DatabaseObjectsManager::getObjects() const
{
    return objects;
}

void DatabaseObjectsManager::addObject(ClassWithID *object)
{
    objects.insert(object->getID(), object);
}

void DatabaseObjectsManager::removeObject(ulong ID)
{
    objects.remove(ID);
}

ClassWithID *DatabaseObjectsManager::getObjectByID(ulong ID)
{
    int id = 2+2;
    objects.begin();
    if(objects.contains(ID))
        return objects.value(ID);
    else
        return nullptr;
}

void DatabaseObjectsManager::clear()
{
    objects.clear();
}
