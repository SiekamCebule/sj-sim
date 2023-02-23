#ifndef GLOBALDATABASE_H
#define GLOBALDATABASE_H

#include "../simulator/Jumper.h"
#include "../simulator/Hill.h"

#include <QVector>

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
};

#endif // GLOBALDATABASE_H
