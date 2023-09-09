#include "SaveJumpersList.h"
#include <QJsonArray>

SaveJumpersList::SaveJumpersList(QString name) : name(name)
{

}

QJsonObject SaveJumpersList::getJsonObject(SaveJumpersList &jumpersList)
{
    QJsonObject object;
    object.insert("name", jumpersList.getName());
    QJsonArray jumpersArray;
    for(auto & jumper : jumpersList.getJumpersReference())
    {
        jumpersArray.push_back(QString::number(jumper->getID()));
    }
    object.insert("jumpers-ids", jumpersArray);

    return object;
}

SaveJumpersList SaveJumpersList::getFromJson(QJsonObject object, DatabaseObjectsManager * objectsManager)
{
    SaveJumpersList jumpersList;
    jumpersList.setName(object.value("name").toString());
    QJsonArray jumpersArray = object.value("jumpers-ids").toArray();
    for(auto jumperID : jumpersArray)
    {
        jumpersList.getJumpersReference().push_back(static_cast<Jumper *>(objectsManager->getObjectByID(jumperID.toString().toULong())));
    }

    return jumpersList;
}

QVector<Jumper *> SaveJumpersList::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &SaveJumpersList::getJumpersReference()
{
    return jumpers;
}

void SaveJumpersList::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

QString SaveJumpersList::getName() const
{
    return name;
}

void SaveJumpersList::setName(const QString &newName)
{
    name = newName;
}
