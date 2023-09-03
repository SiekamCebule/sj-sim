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

    QSet<ulong> getValues() const;
    void reset();

    bool getFreezed() const;
    void setFreezed(bool newFreezed);

private:
    QSet<ulong> values;
    ulong lastID;
    bool freezed;
};

#endif // IDGENERATOR_H
