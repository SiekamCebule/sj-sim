#ifndef GLOBALDATABASE_H
#define GLOBALDATABASE_H

#include "../simulator/Jumper.h"
#include "../simulator/Hill.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"

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

    bool loadJumpers();
    bool loadHills();

    bool writeJumpers();
    bool writeHills();

    void setupJumpersFlags();
    void setupHillsFlags();

public:
    GlobalDatabase(GlobalDatabase &) = delete;
    static GlobalDatabase * get();

    void removeJumper(int index);

    bool loadFromJson();
    bool writeToJson();

    QVector<Jumper> getGlobalJumpers() const;
    void setGlobalJumpers(const QVector<Jumper> &newGlobalJumpers);
    QVector<Jumper> & getEditableGlobalJumpers();
    QVector<Hill> getGlobalHills() const;
    void setGlobalHills(const QVector<Hill> &newGlobalHills);
    QVector<Hill> & getEditableGlobalHills();
};

#endif // GLOBALDATABASE_H
