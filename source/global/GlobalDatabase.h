#ifndef GLOBALDATABASE_H
#define GLOBALDATABASE_H

#include "../simulator/Jumper.h"
#include "../simulator/Hill.h"
#include "../competitions/CompetitionRules.h"
#include "../seasons/SimulationSave.h"
#include "PointsForPlacesPreset.h"

#include <QVector>
#include <QStringList>

class GlobalDatabase
{
private:
    GlobalDatabase(); 
    ~GlobalDatabase();
    static GlobalDatabase * m_globalDatabase;

    QVector<Jumper> globalJumpers;
    QVector<Hill> globalHills;
    QVector<CompetitionRules> globalCompetitionsRules;
    QVector<SimulationSave> globalSimulationSaves;
    QVector<PointsForPlacesPreset> globalPointsForPlacesPresets;

public:
    GlobalDatabase(GlobalDatabase &) = delete;
    static GlobalDatabase * get();

    void removeJumper(int index);

    bool loadFromJson();
    bool writeToJson();

    bool loadJumpers();
    bool loadHills();
    bool loadCompetitionsRules();
    bool loadSimulationSaves();
    bool loadPointsForPlacesPresets();

    bool writeJumpers();
    bool writeHills();
    bool writeCompetitionsRules();
    bool writeSimulationSaves();
    bool writePointsForPlacesPresets();

    void setupJumpersFlags();
    void setupHillsFlags();

    QVector<Jumper> getGlobalJumpers() const;
    void setGlobalJumpers(const QVector<Jumper> &newGlobalJumpers);
    QVector<Jumper> & getEditableGlobalJumpers();
    QVector<Hill> getGlobalHills() const;
    void setGlobalHills(const QVector<Hill> &newGlobalHills);
    QVector<Hill> & getEditableGlobalHills();
    QVector<CompetitionRules> getGlobalCompetitionsRules() const;
    void setGlobalCompetitionsRules(const QVector<CompetitionRules> &newGlobalCompetitionsRules);
    QVector<CompetitionRules> & getEditableCompetitionRules();
    QVector<SimulationSave> getGlobalSimulationSaves() const;
    QVector<SimulationSave> & getEditableGlobalSimulationSaves();
    void setGlobalSimulationSaves(const QVector<SimulationSave> &newGlobalSimulationSaves);
    QVector<PointsForPlacesPreset> getGlobalPointsForPlacesPresets() const;
    QVector<PointsForPlacesPreset> & getEditableGlobalPointsForPlacesPresets();
    void setGlobalPointsForPlacesPresets(const QVector<PointsForPlacesPreset> &newGlobalPointsForPlacesPresets);
};

#endif // GLOBALDATABASE_H
