#ifndef ABSTRACTCLASSIFICATION_H
#define ABSTRACTCLASSIFICATION_H

#include <QMap>
#include <QString>
#include <QJsonObject>
#include "../utilities/ClassWithID.h"

class Classification : public ClassWithID
{
public:
    Classification(QString name = "");

    enum PunctationType{
        PointsForPlaces,
        CompetitionPoints
    };

    enum ClassificationType{
        Individual,
        Team
    };

    static Classification getFromJson(QJsonObject obj);
    static QJsonObject getJsonObject(Classification & classification);

private:
    QString name;
    short classificationType;
    short punctationType;
    QMap<int, int> pointsForPlaces;

public:
    short getClassificationType() const;
    void setClassificationType(short newClassificationType);
    QMap<int, int> getPointsForPlaces() const;
    QMap<int, int> &getPointsForPlacesReference();
    void setPointsForPlaces(const QMap<int, int> &newPointsForPlaces);
    QString getName() const;
    void setName(const QString &newName);
    short getPunctationType() const;
    void setPunctationType(short newPunctationType);
};

#endif // ABSTRACTCLASSIFICATION_H
