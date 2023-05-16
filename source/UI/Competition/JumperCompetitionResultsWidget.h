#ifndef JUMPERCOMPETITIONRESULTSWIDGET_H
#define JUMPERCOMPETITIONRESULTSWIDGET_H

#include <QWidget>
#include "../../competitions/CompetitionResults.h"

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
    CompetitionSingleResult * jumperResult;

public:
    CompetitionSingleResult *getJumperResult() const;
    void setJumperResult(CompetitionSingleResult *newJumperResult);
};

#endif // JUMPERCOMPETITIONRESULTSWIDGET_H
