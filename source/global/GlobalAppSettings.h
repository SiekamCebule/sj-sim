#ifndef GLOBALAPPSETTINGS_H
#define GLOBALAPPSETTINGS_H

#include "JumpDataInfoChoice.h"

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
    bool showCalendarEditorHelp;
    bool showSeasonJumpersAndHillsHelp;
    JumpDataInfoChoice jumpDataInfoChoice;

public:
    short getLanguageID() const;
    void setLanguageID(const short &newLanguageID);

    enum Language{
        Polish,
        English
    };

    bool loadFromJson();
    bool writeToJson();
    bool getShowCalendarEditorHelp() const;
    void setShowCalendarEditorHelp(bool newShowCalendarEditorHelp);
    bool getShowSeasonJumpersAndHillsHelp() const;
    void setShowSeasonJumpersAndHillsHelp(bool newShowSeasonJumpersAndHillsHelp);
    JumpDataInfoChoice getJumpDataInfoChoice() const;
    JumpDataInfoChoice & getJumpDataInfoChoiceReference();
    void setJumpDataInfoChoice(const JumpDataInfoChoice &newJumpDataInfoChoice);
};

#endif // GLOBALAPPSETTINGS_H
