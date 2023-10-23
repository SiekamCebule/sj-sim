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

    QString jumpInfoWebhook;
    QString competitionSingleResultWebhook;
    QString competitionResultsWebhook;

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
    QString getJumpInfoWebhook() const;
    void setJumpInfoWebhook(const QString &newJumpInfoWebhook);
    QString getCompetitionSingleResultWebhook() const;
    void setCompetitionSingleResultWebhook(const QString &newCompetitionSingleResultWebhook);
    QString getCompetitionResultsWebhook() const;
    void setCompetitionResultsWebhook(const QString &newCompetitionResultsWebhook);
};

#endif // GLOBALAPPSETTINGS_H
