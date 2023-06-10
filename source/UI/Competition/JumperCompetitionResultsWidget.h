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
    bool positionShowing;

public:
    CompetitionSingleResult *getJumperResult() const;
    void setJumperResult(CompetitionSingleResult *newJumperResult);
    bool getPositionShowing() const;
    void setPositionShowing(bool newPositionShowing);
};

#endif // JUMPERCOMPETITIONRESULTSWIDGET_H
