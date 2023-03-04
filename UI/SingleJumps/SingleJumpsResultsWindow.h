#ifndef SINGLEJUMPSRESULTSWINDOW_H
#define SINGLEJUMPSRESULTSWINDOW_H

#include <QDialog>
#include "../../single-jumps/SingleJumpsManager.h"

class QSplineSeries;

namespace Ui {
class SingleJumpsResultsWindow;
}

class SingleJumpsResultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SingleJumpsResultsWindow(QWidget *parent = nullptr);
    ~SingleJumpsResultsWindow();

    void fillJumperInfo();
    void fillHillInfo();
    void fillMiniJumpsResultsLayout();
    void fillDistancesChart();
    void fillJudgesChart();

    SingleJumpsManager *getManager() const;
    void setManager(SingleJumpsManager *newManager);
    int getMaxNumberOfDistancesForChart() const;
    int getMaxNumberOfJudgesForChart() const;

private:
    Ui::SingleJumpsResultsWindow *ui;

    SingleJumpsManager * manager;

    QSplineSeries * getSplineSeriesForDistancesChart();
    int maxNumberOfDistancesForChart;
    QSplineSeries * getSplineSeriesForJudgesChart();
    int maxNumberOfJudgesForChart;
};

#endif // SINGLEJUMPSRESULTSWINDOW_H
