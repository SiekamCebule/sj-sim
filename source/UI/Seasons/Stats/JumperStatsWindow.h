#ifndef JUMPERSTATSWINDOW_H
#define JUMPERSTATSWINDOW_H

#include <QDialog>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include "FilteringWidgets/CompetitionsRangeComboBoxesWidget.h"
#include "FilteringWidgets/SerieTypesComboBoxesWidget.h"

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
private:
    Ui::JumperStatsWindow *ui;
    CompetitionsRangeComboBoxesWidget * rangeComboBoxes;
    SerieTypesComboBoxesWidget * serieTypesCheckBoxes;

    Jumper * jumper;

    //Jumper apperances
    QChart * jumperApperancesChart;
    QChartView * jumperApperancesChartView;
    QLineSeries * jumperApperancesLineSeries;

    void setupCharts();

public:
    CompetitionsRangeComboBoxesWidget *getRangeComboBoxes() const;
    void setRangeComboBoxes(CompetitionsRangeComboBoxesWidget *newRangeComboBoxes);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
private slots:
    void on_horizontalScrollBar_jumperStatsChartScrollBar_valueChanged(int value);
};

#endif // JUMPERSTATSWINDOW_H
