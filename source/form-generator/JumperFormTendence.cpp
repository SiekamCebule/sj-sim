#include "JumperFormTendence.h"

JumperFormTendence::JumperFormTendence(Jumper *jumper, double tendence) : jumper(jumper), tendence(tendence)
{

}

int JumperFormTendence::getTendenceType()
{
    if(tendence > 0)
        return Increase;
    else if(tendence < 0)
        return Decrease;
    else
        return Neutral;
}

QJsonObject JumperFormTendence::getJsonObject(JumperFormTendence &tendence)
{
    QJsonObject object;
    object.insert("jumper-id", QString::number(tendence.getJumper()->getID()));
    object.insert("tendence", tendence.getTendence());

    return object;
}

JumperFormTendence JumperFormTendence::getFromJson(QJsonObject obj, DatabaseObjectsManager * objectsManager)
{
    JumperFormTendence tendence;
    tendence.setJumper(static_cast<Jumper *>(objectsManager->getObjectByID(obj.value("jumper-id").toString().toULong())));
    tendence.setTendence(obj.value("tendence").toDouble());
    return tendence;
}

double & JumperFormTendence::getTendence()
{
    return tendence;
}

void JumperFormTendence::setTendence(double newTendence)
{
    tendence = newTendence;
}

Jumper *JumperFormTendence::getJumper() const
{
    return jumper;
}

void JumperFormTendence::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}
