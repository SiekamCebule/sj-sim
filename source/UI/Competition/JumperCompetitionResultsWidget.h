#ifndef JUMPERCOMPETITIONRESULTSWIDGET_H
#define JUMPERCOMPETITIONRESULTSWIDGET_H

#include <QWidget>
#include "../../competitions/IndividualCompetitions/IndividualCompetitionResults.h"

namespace Ui {
class JumperCompetitionResultsWidget;
}

class JumperCompetitionResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JumperCompetitionResultsWidget(QWidget *parent = nullptr);
    ~JumperCompetitionResultsWidget();

    void fillWidget();

private:
    Ui::JumperCompetitionResultsWidget *ui;
    IndividualCompetitionSingleResult * jumperResult;

public:
    IndividualCompetitionSingleResult *getJumperResult() const;
    void setJumperResult(IndividualCompetitionSingleResult *newJumperResult);
};

#endif // JUMPERCOMPETITIONRESULTSWIDGET_H
