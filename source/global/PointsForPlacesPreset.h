#ifndef POINTSFORPLACESPRESET_H
#define POINTSFORPLACESPRESET_H

#include <QString>
#include <QMap>
#include <QJsonObject>

class PointsForPlacesPreset
{
public:
    PointsForPlacesPreset(QString name = "");
private:
    QString name;
    QMap<int, double> pointsForPlaces; //1. Miejsce, 2. Punkty
public:
    QString getName() const;
    void setName(const QString &newName);
    QMap<int, double> getPointsForPlaces() const;
    QMap<int, double> & getPointsForPlacesReference();
    void setPointsForPlaces(const QMap<int, double> &newPointsForPlaces);
public:
    static QJsonObject getPointsForPlacesPresetJsonObject(PointsForPlacesPreset & preset);
    static QVector<PointsForPlacesPreset> getPointsForPlacesPresetsVectorFromJson(const QByteArray & bytes);
};

#endif // POINTSFORPLACESPRESET_H
