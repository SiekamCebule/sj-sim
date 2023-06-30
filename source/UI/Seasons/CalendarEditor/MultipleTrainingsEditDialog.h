#ifndef MULTIPLETRAININGSEDITDIALOG_H
#define MULTIPLETRAININGSEDITDIALOG_H

#include <QDialog>
#include "../../EditorWidgets/CompetitionRulesEditorWidget.h"

namespace Ui {
class MultipleTrainingsEditDialog;
}

class MultipleTrainingsEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultipleTrainingsEditDialog(QWidget *parent = nullptr);
    ~MultipleTrainingsEditDialog();

    int getTrainingsCount();
    CompetitionRules getTrainingsRules();

signals:
    void submitted();

private slots:
    void on_pushButton_trainingsRules_clicked();
    void on_pushButton_submit_clicked();

private:
    Ui::MultipleTrainingsEditDialog *ui;
    CompetitionRules trainingsRules;
    QVector<CompetitionRules> * rulesList;
public:
    QVector<CompetitionRules> *getRulesList() const;
    void setRulesList(QVector<CompetitionRules> *newRulesList);
};

#endif // MULTIPLETRAININGSEDITDIALOG_H
