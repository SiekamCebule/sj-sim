#include "CountryFlagsManager.h"

#include <QPixmap>
#include <QString>
#include <QDebug>

CountryFlagsManager::CountryFlagsManager()
{

}

QPixmap CountryFlagsManager::getFlagPixmap(const QString &twoLettersCountryCode)
{
    return QPixmap("flags/" + twoLettersCountryCode + ".svg");
}

QString CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(const QString &code)
{
    if(code == "afg") return "af";
    else if(code == "alb") return "al";
    else if(code == "dza") return "dz";
    else if(code == "asm") return "as";
    else if(code == "and") return "ad";
    else if(code == "ago") return "ao";
    else if(code == "aia") return "ai";
    else if(code == "ata") return "aq";
    else if(code == "atg") return "ag";
    else if(code == "arg") return "ar";
    else if(code == "arm") return "am";
    else if(code == "abw") return "aw";
    else if(code == "aus") return "au";
    else if(code == "aut") return "at";
    else if(code == "aze") return "az";
    else if(code == "bhs") return "bs";
    else if(code == "bhr") return "bh";
    else if(code == "bgd") return "bd";
    else if(code == "brb") return "bb";
    else if(code == "blr") return "by";
    else if(code == "bel") return "be";
    else if(code == "blz") return "bz";
    else return "xx";
}
