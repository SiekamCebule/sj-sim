#ifndef SAVEJUMPERSLIST_H
#define SAVEJUMPERSLIST_H

#include <QVector>
#include <QJsonObject>
#include <QString>
#include "../simulator/Jumper.h"
#include "../global/DatabaseObjectsManager.h"

class SaveJumpersList
{
public:
    SaveJumpersList(QString name = "");

    static QJsonObject getJsonObject(SaveJumpersList & jumpersList);
    static SaveJumpersList getFromJson(QJsonObject object, DatabaseObjectsManager *objectsManager);

private:
    QVector<Jumper *> jumpers;
    QString name;
public:
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper *> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    QString getName() const;
    void setName(const QString &newName);
};

#endif // SAVEJUMPERSLIST_H
