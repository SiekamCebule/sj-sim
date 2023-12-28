#include "SimulationSave.h"
#include <QFile>
#include "../global/Uuid.h"
#include <QtConcurrent>

extern Uuid globalIDGenerator;

SimulationSave::SimulationSave() :
    Identifiable()
{
    actualSeason = nullptr;
}

SimulationSave::~SimulationSave()
{
    for(auto & hill : hills)
        delete hill;
    for(auto & jumper : jumpers)
        delete jumper;
    for(auto & season : seasons)
    {
        for(auto & cal : season.getCalendarsReference())
        {
            for(auto & competition : cal->getCompetitionsReference())
                delete competition;
            for(auto & classification : cal->getClassificationsReference())
                delete classification;
        }
    }
}

SimulationSave * SimulationSave::getFromJson(QJsonObject obj, IdentifiableObjectsStorage * storage)
{
    SimulationSave * save = new SimulationSave();
    if(storage == nullptr)
    {
        storage = save;
    }
    save->setID(sole::rebuild(obj.value("id").toString().toStdString()));

    qDebug()<<"id: "<<save->getID();

    QJsonArray array = obj.value("jumpers").toArray();
    QVector<QJsonValue> values;
    for(auto val : qAsConst(array))
        values.push_back(val);
    QFuture<Jumper *> jumpersFuture = QtConcurrent::mapped(values, [storage](const QJsonValue & value){
        return new Jumper(Jumper::getFromJson(value.toObject()));
    });
    save->setJumpers(jumpersFuture.results().toVector());
    storage->add(save->getJumpersReference());

    qDebug()<<"jumpers";

    array = obj.value("hills").toArray();
    values.clear();
    for(auto val : qAsConst(array))
        values.push_back(val);
    QFuture<Hill *> hillsFuture = QtConcurrent::mapped(values, [storage](const QJsonValue & value){
        return new Hill(Hill::getFromJson(value.toObject()));
    });
    save->setHills(hillsFuture.results().toVector());
    storage->add(save->getHillsReference());

    array = obj.value("rules").toArray();
    values.clear();
    for(auto val : qAsConst(array))
        values.push_back(val);
    QFuture<CompetitionRules> rulesFuture = QtConcurrent::mapped(values, [storage](const QJsonValue & value){
        return CompetitionRules::getFromJson(value.toObject());
    });
    save->setCompetitionRules(rulesFuture.results().toVector());
    storage->add(save->getCompetitionRulesReference());

    qDebug()<<"rules";

    QJsonArray seasonsArray = obj.value("seasons").toArray();
    values.clear();
    for(auto val : seasonsArray)
        values.push_back(val);
    QFuture<Season> seasonsFuture = QtConcurrent::mapped(values, [storage](const QJsonValue &value){
        return Season::getFromJson(value.toObject(), storage);
    });
    save->setSeasons(seasonsFuture.results().toVector());
    storage->add(save->getSeasonsReference());
        /*Season season = Season::getFromJson(val.toObject(), objectsManager);
        save->getSeasonsReference().push_back(season);*/


    array = obj.value("jumpers-form-instabilities").toArray();
    values.clear();
    for(auto val : qAsConst(array))
        values.push_back(val);
    QFuture<QPair<Jumper *, double>> instFuture = QtConcurrent::mapped(values, [storage](const QJsonValue & value){
        QPair<Jumper *, double> pair;
        pair.first = static_cast<Jumper *>(storage->get(value.toObject().value("jumper-id").toString()));
        pair.second = value.toObject().value("instability").toString().toDouble();
        return pair;
    });
    QHash<Jumper *, double> instabilitiesHash;
    for(auto & pair : instFuture.results())
        instabilitiesHash.insert(pair.first, pair.second);
    save->setJumpersFormInstabilities(instabilitiesHash);
    qDebug()<<save->getJumpersFormInstabilitiesReference().count();
    qDebug()<<save->getJumpersFormInstabilitiesReference().value(save->getJumpersReference().first());

    array = obj.value("jumpers-lists").toArray();
    values.clear();
    for(auto & val : qAsConst(array))
        values.push_back(val);
    QFuture<SaveJumpersList> listsFuture = QtConcurrent::mapped(values, [storage](const QJsonValue & value){
        return SaveJumpersList::getFromJson(value.toObject(), storage);
    });
    save->setJumpersLists(listsFuture.results());

    save->setActualSeason(static_cast<Season *>(storage->get(obj.value("actual-season-id").toString())));
    save->setNextCompetitionIndex(obj.value("next-competition-index").toInt());
    if(save->getActualSeason()->getActualCalendar() != nullptr && save->getActualSeason()->getActualCalendar()->getCompetitionsReference().count() > 0)
        save->setNextCompetition(save->getActualSeason()->getActualCalendar()->getCompetitionsReference()[save->getNextCompetitionIndex()]);
    else
        save->setNextCompetition(nullptr);

    save->setShowForm(obj.value("show-form").toBool(true));
    save->setShowInstability(obj.value("show-instability").toBool(true));
    save->setSaveFileSizeReduce(obj.value("save-file-size-reduce").toBool(false));

    return save;
}

QJsonObject SimulationSave::getJsonObject(SimulationSave &save)
{
    QJsonObject object;
    object.insert("id", save.getIDStr());

    QFuture<QJsonObject> jumpersFuture = QtConcurrent::mapped(save.getJumpersReference(), [](Jumper * p){return Jumper::getJsonObject(*p);});
    QJsonArray jumpersArray;
    for(auto & o : jumpersFuture.results())
        jumpersArray.append(o);
    object.insert("jumpers", jumpersArray);

    QFuture<QJsonObject> hillsFuture = QtConcurrent::mapped(save.getHillsReference(), [](Hill * p){return Hill::getJsonObject(*p);});
    QJsonArray hillsArray;
    for(auto & o : hillsFuture.results())
        hillsArray.append(o);
    object.insert("hills", hillsArray);

    QFuture<QJsonObject> rulesFuture = QtConcurrent::mapped(save.getCompetitionRulesReference(), [](const CompetitionRules & p){return CompetitionRules::getJsonObject(p);});
    QJsonArray rulesArray;
    for(auto & o : rulesFuture.results())
        rulesArray.append(o);
    object.insert("rules", rulesArray);

    QFuture<QJsonObject> seasonsFuture = QtConcurrent::mapped(save.getSeasonsReference(), [](const Season & p){return Season::getJsonObject(p);});
    QJsonArray seasonsArray;
    for(auto & o : seasonsFuture.results())
        seasonsArray.append(o);
    object.insert("seasons", seasonsArray);

    QFuture<QJsonObject> instabilitiesFuture = QtConcurrent::mapped(save.getJumpersFormInstabilitiesToVector(), [](QPair<Jumper *, double> p){
        QJsonObject o;
        o.insert("jumper-id", p.first->getIDStr());
        o.insert("instability", QString::number(p.second));
        return o;
    });
    QJsonArray instabilitiesArray;
    for(auto & o : instabilitiesFuture.results())
        instabilitiesArray.append(o);
    object.insert("jumpers-form-instabilities", instabilitiesArray);

    QFuture<QJsonObject> listsFuture = QtConcurrent::mapped(save.getJumpersListsReference(), [](const SaveJumpersList & p){return SaveJumpersList::getJsonObject(p);});
    QJsonArray listsArray;
    for(auto & o : listsFuture.results())
        listsArray.append(o);
    object.insert("jumpers-lists", listsArray);

    object.insert("actual-season-id", save.getActualSeason()->getIDStr());
    object.insert("next-competition-index", save.getNextCompetitionIndex());

    object.insert("show-form", save.getShowForm());
    object.insert("show-instability", save.getShowInstability());
    object.insert("save-file-size-reduce", save.getSaveFileSizeReduce());

    return object;
}

bool SimulationSave::saveToFile(QString dir, QString fileName)
{
    if(fileName == "!default")
        fileName = getName();
    QJsonDocument document;
    QJsonObject mainObject;
    repairDatabase();
    mainObject.insert("simulation-save", SimulationSave::getJsonObject(*this));
    document.setObject(mainObject);

    QFile file(dir + fileName + ".json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji " + getName(), "Nie udało się otworzyć pliku " + dir + getName() + ".json" + "\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    if(saveFileSizeReduce == true)
        file.write(document.toJson(QJsonDocument::Compact));
    else
        file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

SimulationSave *SimulationSave::loadFromFile(QString fileName)
{
    IdentifiableObjectsStorage objectsManager;
        QFile file("simulationSaves/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji", "Nie udało się otworzyć pliku simulationSaves/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        QJsonObject object = doc.object().value("simulation-save").toObject();
        SimulationSave * s = SimulationSave::getFromJson(object, nullptr);
        fileName.chop(5);
        s->setName(fileName);
        return s;
}

void SimulationSave::updateNextCompetitionIndex()
{
    nextCompetitionIndex = 0;
    if(actualSeason->getActualCalendar() != nullptr){
    for(auto & comp : getActualSeason()->getActualCalendar()->getCompetitionsReference())
    {
        if(comp->getPlayed() == false)
            break;
        nextCompetitionIndex++;
    }
    if(nextCompetitionIndex == actualSeason->getActualCalendar()->getCompetitionsReference().count())
        nextCompetition = nullptr;
    else
        nextCompetition = getActualSeason()->getActualCalendar()->getCompetitionsReference()[nextCompetitionIndex];
        }
}

void SimulationSave::repairDatabase()
{
    QVector<Identifiable *> objects;
    for(auto & jumper : jumpers)
        objects.push_back(jumper);
    for(auto & hill : hills)
        objects.push_back(hill);
    for(auto & rules : competitionRules)
        objects.push_back(&rules);
    for(auto & season : seasons)
    {
        for(auto & cal : season.getCalendarsReference())
        {
            for(auto & competition : cal->getCompetitionsReference())
            {
                objects.push_back(competition);
                objects.push_back(&competition->getResultsReference());
                for(auto & groups : competition->getRoundsKOGroupsReference())
                {
                    for(auto & group : groups)
                        objects.push_back(&group);
                }
                for(auto & team : competition->getTeamsReference())
                    objects.push_back(&team);
                for(auto & result : competition->getResultsReference().getResultsReference())
                    objects.push_back(&result);
            }
            for(auto & classification : cal->getClassificationsReference())
            {
                objects.push_back(classification);
                for(auto & result : classification->getResultsReference())
                {
                    objects.push_back(result);
                }
            }
        }
    }

    //globalIDGenerator.reset();
    for(auto & object : objects)
    {
        object->reassign();
    }

    return;
}

void SimulationSave::fixJumpersFormInstabilities()
{
    for(auto & j : jumpers)
    {
        if(getJumperFormInstability(j) == nullptr)
            jumpersFormInstabilities.insert(j, 0);
    }
}

double * SimulationSave::getJumperFormInstability(Jumper * j)
{
    if(jumpersFormInstabilities.contains(j) == false)
        return nullptr;
    else
        return &jumpersFormInstabilities[j];
}

bool SimulationSave::getShowInstability() const
{
    return showInstability;
}

void SimulationSave::setShowInstability(bool newShowInstability)
{
    showInstability = newShowInstability;
}

QVector<QPair<Jumper *, double> > SimulationSave::getJumpersFormInstabilitiesToVector()
{
    QVector<QPair<Jumper *, double>> vec;
    for(auto & key : jumpersFormInstabilities.keys())
    {
        QPair<Jumper *, double> p;
        p.first = key;
        p.second = jumpersFormInstabilities.value(key);
        vec.push_back(p);
    }
    return vec;
}

void SimulationSave::setJumpersFormInstabilities(const QHash<Jumper *, double> &newJumpersFormInstabilities)
{
    jumpersFormInstabilities = newJumpersFormInstabilities;
}

QVector<SaveJumpersList> SimulationSave::getJumpersLists() const
{
    return jumpersLists;
}

QVector<SaveJumpersList> &SimulationSave::getJumpersListsReference()
{
    return jumpersLists;
}

void SimulationSave::setJumpersLists(const QVector<SaveJumpersList> &newJumpersLists)
{
    jumpersLists = newJumpersLists;
}

QHash<Jumper *, double> &SimulationSave::getJumpersFormInstabilitiesReference()
{
    return jumpersFormInstabilities;
}

bool SimulationSave::getSaveFileSizeReduce() const
{
    return saveFileSizeReduce;
}

void SimulationSave::setSaveFileSizeReduce(bool newSaveFileSizeReduce)
{
    saveFileSizeReduce = newSaveFileSizeReduce;
}

bool SimulationSave::getShowForm() const
{
    return showForm;
}

void SimulationSave::setShowForm(bool newShowForm)
{
    showForm = newShowForm;
}

CompetitionInfo *SimulationSave::getNextCompetition() const
{
    return nextCompetition;
}

void SimulationSave::setNextCompetition(CompetitionInfo *newNextCompetition)
{
    nextCompetition = newNextCompetition;
}

int SimulationSave::getNextCompetitionIndex() const
{
    return nextCompetitionIndex;
}

void SimulationSave::setNextCompetitionIndex(int newNextCompetitionIndex)
{
    nextCompetitionIndex = newNextCompetitionIndex;
}

Season *SimulationSave::getActualSeason() const
{
    return actualSeason;
}

void SimulationSave::setActualSeason(Season *newActualSeason)
{
    actualSeason = newActualSeason;
}

void SimulationSave::setCompetitionRules(const QVector<CompetitionRules> &newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

QVector<Hill *> SimulationSave::getHills() const
{
    return hills;
}

QVector<CompetitionRules> &SimulationSave::getCompetitionRulesReference()
{
    return competitionRules;
}

void SimulationSave::setHills(const QVector<Hill *> &newHills)
{
    hills = newHills;
}

QVector<Jumper *> SimulationSave::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &SimulationSave::getJumpersReference()
{
    return jumpers;
}

void SimulationSave::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

QVector<Hill *> &SimulationSave::getHillsReference()
{
    return hills;
}

QVector<Season> SimulationSave::getSeasons() const
{
    return seasons;
}

QVector<Season> &SimulationSave::getSeasonsReference()
{
    return seasons;
}

void SimulationSave::setSeasons(const QVector<Season> &newSeasons)
{
    seasons = newSeasons;
}

QString SimulationSave::getName() const
{
    return name;
}

void SimulationSave::setName(const QString &newName)
{
    name = newName;
}
