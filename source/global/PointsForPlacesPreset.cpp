#include "PointsForPlacesPreset.h"
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QByteArray>

PointsForPlacesPreset::PointsForPlacesPreset(QString name) : name(name)
{

}

QMap<int, int> PointsForPlacesPreset::getPointsForPlaces() const
{
    return pointsForPlaces;
}

QMap<int, int> &PointsForPlacesPreset::getPointsForPlacesReference()
{
    return pointsForPlaces;
}

void PointsForPlacesPreset::setPointsForPlaces(const QMap<int, int> &newPointsForPlaces)
{
    pointsForPlaces = newPointsForPlaces;
}

QJsonObject PointsForPlacesPreset::getPointsForPlacesPresetJsonObject(PointsForPlacesPreset &preset)
{
    QJsonObject object;
    object.insert("name", preset.getName());
    QJsonArray pointsForPlacesArray;
    for(auto key : preset.getPointsForPlacesReference().keys()){
        pointsForPlacesArray.push_back(preset.getPointsForPlacesReference().value(key));
    }
    object.insert("pointsForPlaces", pointsForPlacesArray);
    return object;
}

QVector<PointsForPlacesPreset> PointsForPlacesPreset::getPointsForPlacesPresetsVectorFromJson(const QByteArray &bytes)
{
    QVector<PointsForPlacesPreset> presets;
    presets.clear();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu presetów punktów za miejsca", "Nie udało się wczytać presetów z pliku userData/GlobalDatabase/globalPointsForPlacesPresets.json\nTreść błędu: " + error.errorString(),  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return presets;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("pointsForPlacesPresets");
    if(!value.isArray())
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu presetów punktów za miejsca","Prawdopodobnie w tym pliku nie ma listy z presetami\nUpewnij się, że wybrałeś właściwy plik!");
        message.setModal(true);
        message.exec();
        return presets;
    }
    QJsonArray array = value.toArray();
    for(const auto & val : array){
        QJsonObject obj = val.toObject();
        PointsForPlacesPreset preset;
        preset.setName(obj.value("name").toString());
        QJsonArray pointsForPlacesArray = obj.value("pointsForPlaces").toArray();
        int i=1;
        for(const auto & val : pointsForPlacesArray){
            preset.getPointsForPlacesReference().insert(i, val.toInt());
            i++;
        }
        presets.push_back(preset);
    }
    return presets;
}

QString PointsForPlacesPreset::getName() const
{
    return name;
}

void PointsForPlacesPreset::setName(const QString &newName)
{
    name = newName;
}
