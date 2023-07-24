#ifndef MULTIPLETRIALROUNDSEDITDIALOG_H
#define MULTIPLETRIALROUNDSEDITDIALOG_H

#include <QDialog>
#include "../../EditorWidgets/CompetitionRulesEditorWidget.h"


namespace Ui {
class MultipleTrialRoundsEditDialog;
}

class MultipleTrialRoundsEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultipleTrialRoundsEditDialog(QWidget *parent = nullptr);
    ~MultipleTrialRoundsEditDialog();

    bool getTrialRoundChecked();
    CompetitionRules getTrialRoundRules();

signals:
    void submitted();

private slots:
    void on_pushButton_trainingsRules_clicked();

    void on_pushButton_submit_clicked();

private:
    Ui::MultipleTrialRoundsEditDialog *ui;
    CompetitionRules trialRoundRules;
    QVector<CompetitionRules> * rulesList;
public:
    QVector<CompetitionRules> *getRulesList() const;
    void setRulesList(QVector<CompetitionRules> *newRulesList);
    void setTrialRoundRules(const CompetitionRules &newTrialRoundRules);
    void setTrialRound(bool ok);
};

#endif // MULTIPLETRIALROUNDSEDITDIALOG_H
