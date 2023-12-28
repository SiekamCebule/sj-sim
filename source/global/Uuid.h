#ifndef UUID_H
#define UUID_H

#include <QSet>
#include "source/3rdparty/sole.hpp"

class Uuid
{
public:
    static sole::uuid v4()
    {
        return sole::uuid4();
    }
};

#endif // UUID_H
