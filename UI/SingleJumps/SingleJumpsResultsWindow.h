#ifndef SINGLEJUMPSRESULTSWINDOW_H
#define SINGLEJUMPSRESULTSWINDOW_H

#include <QDialog>
#include "../../single-jumps/SingleJumpsManager.h"

class QSplineSeries;
class QBarSeries;
class SingleJumpMiniResultWidget;

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
    void fillLandingsChart();

    SingleJumpsManager *getManager() const;
    void setManager(SingleJumpsManager *newManager);
    int getMaxNumberOfDistancesForChart() const;
    int getMaxNumberOfJudgesForChart() const;
    int getMaxNumberOfLandingsForChart() const;
    int getSelectedItemIndex() const;
    void setSelectedItemIndex(int newSelectedItemIndex);
    QVector<SingleJumpMiniResultWidget *> getMiniResultItems() const;
    void setMiniResultItems(const QVector<SingleJumpMiniResultWidget *> &newMiniResultItems);

private:
    Ui::SingleJumpsResultsWindow *ui;

    SingleJumpsManager * manager;

    int selectedItemIndex;
    QVector<SingleJumpMiniResultWidget *> miniResultItems;

    QSplineSeries * getSplineSeriesForDistancesChart();
    int maxNumberOfDistancesForChart;
    QSplineSeries * getSplineSeriesForJudgesChart();
    int maxNumberOfJudgesForChart;
    QBarSeries * getBarSeriesForLandingsChar();
    int maxNumberOfLandingsForChart;
};

#endif // SINGLEJUMPSRESULTSWINDOW_H
