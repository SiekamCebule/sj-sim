#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <QSet>

class IDGenerator
{
public:
    IDGenerator();

    bool hasValue(ulong value);

    ulong generateNewID();
    bool removeID(ulong ID);
    void addExistingID(ulong ID);
    ulong getLastID() const;

private:
    QSet<ulong> values;
    ulong lastID;
};

#endif // IDGENERATOR_H
