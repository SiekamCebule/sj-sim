#include "SeasonDatabaseObjectsManager.h"

SeasonDatabaseObjectsManager seasonObjectsManager;

SeasonDatabaseObjectsManager::SeasonDatabaseObjectsManager()
{

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
    return objects.value(ID);
}

void SeasonDatabaseObjectsManager::clear()
{
    objects.clear();
}
