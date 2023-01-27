#ifndef WINDSGENERATOR_H
#define WINDSGENERATOR_H

#include "../simulation/Wind.h"

#include <QVector>
#include <QString>


class WindsGenerator
{
public:

private:
    QVector<Wind> * winds;

    // ustawienia generatora
    double baseWind;
    double windChange;

    QSet<QString> userSettings; // ustawienia uzytkownika

public:
    WindsGenerator(double baseWind = 0, double windChange = 0, const QSet<QString> &userSettings = QSet<QString>(), QVector<Wind> *winds = nullptr);
    QVector<Wind> *getWinds() const;
    void setWinds(QVector<Wind> *newWinds);
    double getBaseWind() const;
    void setBaseWind(double newBaseWind);
    double getWindChange() const;
    void setWindChange(double newWindChange);
    QSet<QString> & getUserSettings();
    void setUserSettings(const QSet<QString> &newUserSettings);
};

#endif // WINDSGENERATOR_H
