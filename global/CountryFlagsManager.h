#ifndef COUNTRYFLAGSMANAGER_H
#define COUNTRYFLAGSMANAGER_H

class QPixmap;
class QString;

class CountryFlagsManager
{
public:
    CountryFlagsManager();

    static QPixmap getFlagPixmap(const QString & twoLettersCountryCode);
    static QString convertThreeLettersCountryCodeToTwoLetters(const QString & code);
};

#endif // COUNTRYFLAGSMANAGER_H
