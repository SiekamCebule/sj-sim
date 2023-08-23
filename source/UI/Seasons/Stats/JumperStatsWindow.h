#ifndef JUMPERSTATSWINDOW_H
#define JUMPERSTATSWINDOW_H

#include <QDialog>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "FilteringWidgets/CompetitionsRangeComboBoxesWidget.h"
#include "FilteringWidgets/SerieTypesComboBoxesWidget.h"
#include "FilteringWidgets/ClassificationsCheckBoxesWidget.h"

namespace Ui {
class JumperStatsWindow;
}

class JumperStatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JumperStatsWindow(QWidget *parent = nullptr);
    ~JumperStatsWindow();

    void setupConnections();
    void fillWindow();

    void fillJumperApperancesChart(QVector<CompetitionSingleResult *> &singleResults);
    void fillJudgesPointsChart(QVector<CompetitionSingleResult *> &singleResults);
    void fillJumperFormChart(QVector<CompetitionSingleResult *> &singleResults);
    void fillTakeoffRatingChart(QVector<CompetitionSingleResult *> &singleResults);
    void fillFlightRatingChart(QVector<CompetitionSingleResult *> &singleResults);
private:
    Ui::JumperStatsWindow *ui;
    CompetitionsRangeComboBoxesWidget * rangeComboBoxes;
    SerieTypesComboBoxesWidget * serieTypesCheckBoxes;
    ClassificationsCheckBoxesWidget * classificationsCheckBoxes;

    Jumper * jumper;

    //Jumper apperances
    QChart * jumperApperancesChart;
    QChartView * jumperApperancesChartView;
    QLineSeries * jumperApperancesLineSeries;
    //Judges points
    QChart * judgesPointsChart;
    QChartView * judgesPointsChartView;
    QLineSeries * judgesPointsLineSeries;
    //Form
    QChart * jumperFormChart;
    QChartView * jumperFormChartView;
    QLineSeries * jumperFormLineSeries;
    //Takeoff rating
    QChart * takeoffRatingChart;
    QChartView * takeoffRatingChartView;
    QLineSeries * takeoffRatingLineSeries;
    //Flight rating
    QChart * flightRatingChart;
    QChartView * flightRatingChartView;
    QLineSeries * flightRatingLineSeries;

    void setupCharts();

    //Charts UI
    //3. Ocena wybicia
    QVBoxLayout * layout_takeoffRatingChart;
    //4. Ocena lotu
    QVBoxLayout * layout_flightRatingChart;

public:
    CompetitionsRangeComboBoxesWidget *getRangeComboBoxes() const;
    void setRangeComboBoxes(CompetitionsRangeComboBoxesWidget *newRangeComboBoxes);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    ClassificationsCheckBoxesWidget *getClassificationsCheckBoxes() const;
    void setClassificationsCheckBoxes(ClassificationsCheckBoxesWidget *newClassificationsCheckBoxes);

private slots:
    void on_horizontalScrollBar_jumperStatsChartScrollBar_valueChanged(int value);
    void on_checkBox_stateChanged(int arg1);
};

#endif // JUMPERSTATSWINDOW_H
