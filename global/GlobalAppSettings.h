#ifndef GLOBALAPPSETTINGS_H
#define GLOBALAPPSETTINGS_H

class QString;

class GlobalAppSettings
{
private:
    GlobalAppSettings();
    static GlobalAppSettings * m_globalAppSettings;

public:
    GlobalAppSettings(GlobalAppSettings &) = delete;
    static GlobalAppSettings * get();

private:
    short languageID;

public:
    short getLanguageID() const;
    void setLanguageID(const short &newLanguageID);

    enum Language{
        Polish,
        English
    };

    bool loadFromJson();
    bool writeToJson();
};

#endif // GLOBALAPPSETTINGS_H
