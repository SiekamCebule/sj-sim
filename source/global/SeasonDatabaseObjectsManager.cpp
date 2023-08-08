#include "SeasonDatabaseObjectsManager.h"

SeasonDatabaseObjectsManager seasonObjectsManager;

SeasonDatabaseObjectsManager::SeasonDatabaseObjectsManager()
{

}

QMap<ulong, ClassWithID *> SeasonDatabaseObjectsManager::getObjects() const
{
    return objects;
}

void SeasonDatabaseObjectsManager::addObject(ClassWithID *object)
{
    objects.insert(object->getID(), object);
}

void SeasonDatabaseObjectsManager::removeObject(ulong ID)
{
    objects.remove(ID);
}

ClassWithID *SeasonDatabaseObjectsManager::getObjectByID(ulong ID)
{
    int id = 2+2;
    objects.begin();
    if(objects.contains(ID))
        return objects.value(ID);
    else
        return nullptr;
}

void SeasonDatabaseObjectsManager::clear()
{
    objects.clear();
}
