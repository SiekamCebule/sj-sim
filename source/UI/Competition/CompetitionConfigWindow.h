#ifndef COMPETITIONCONFIGWINDOW_H
#define COMPETITIONCONFIGWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include "../../simulator/Jumper.h"
#include "../../simulator/Team.h"

class HillEditorWidget;
class WindsGeneratorSettingsEditorWidget;
class CompetitionRulesEditorWidget;
class OtherCompetitionSettingsWidget;
class CompetitionStartListDisplayWidget;

namespace Ui {
class SingleCompetitionConfigWindow;
}

class CompetitionConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionConfigWindow(short type, QWidget *parent = nullptr);
    ~CompetitionConfigWindow();

    enum Type{
        SingleCompetition,
        SeasonCompetition
    };

    int getJumpersCountInTeam() const;

private:
    Ui::SingleCompetitionConfigWindow *ui;
    int type;
    HillEditorWidget * hillEditor;
    WindsGeneratorSettingsEditorWidget * windsGeneratorSettingsEditor;

    void setupHillToolBoxItem();
    //
    CompetitionRulesEditorWidget * competitionRulesEditor;
    QWidget * competitionRulesParentWidget;
    QVBoxLayout * competitionRulesToolBoxItemLayout;
    QVBoxLayout * existingRulesLabelAndComboBoxLayout;
    QLabel * existingCompetitionRulesLabel;
    QComboBox * existingCompetitionRulesComboBox;
    void setupCompetitionRulesToolBoxItem();
    //


    OtherCompetitionSettingsWidget * otherCompetitionSettingsEditor;

    //
    CompetitionStartListDisplayWidget * startListDisplayWidget;
    QWidget * startListParentWidget;
    QVBoxLayout * startListParentLayout;
    QPushButton * pushButton_loadJumpersList;
    //

    QVector<Team> competitionTeams;
    QVector<Jumper> competitionJumpers;

public:
    short getType() const;
    void setType(short newType);
    WindsGeneratorSettingsEditorWidget *getWindGeneratorSettingsEditor() const;
    void setWindGeneratorSettingsEditor(WindsGeneratorSettingsEditorWidget *newWindsGeneratorSettingsEditor);
    CompetitionRulesEditorWidget *getCompetitionRulesEditor() const;
    void setCompetitionRulesEditor(CompetitionRulesEditorWidget *newCompetitionRulesEditor);
    HillEditorWidget *getHillEditor() const;
    void setHillEditor(HillEditorWidget *newHillEditor);
    OtherCompetitionSettingsWidget *getOtherCompetitionSettingsEditor() const;
    void setOtherCompetitionSettingsEditor(OtherCompetitionSettingsWidget *newOtherCompetitionSettingsEditor);
    CompetitionStartListDisplayWidget *getStartListDisplayWidget() const;
    void setStartListDisplayWidget(CompetitionStartListDisplayWidget *newStartListDisplayWidget);
    QVector<Jumper> getCompetitionJumpers() const;
    QVector<Team> getCompetitionTeams() const;
    void setCompetitionTeams(const QVector<Team> &newCompetitionTeams);
    void setCompetitionJumpers(const QVector<Jumper> &newCompetitionJumpers);
private slots:
    void on_pushButton_submit_clicked();
};

#endif // COMPETITIONCONFIGWINDOW_H