#ifndef SIMULATIONRATINGSWINDOW_H
#define SIMULATIONRATINGSWINDOW_H

#include <QDialog>
#include "FilteringWidgets/ClassificationsCheckBoxesWidget.h"
#include "FilteringWidgets/CompetitionsRangeComboBoxesWidget.h"
#include "FilteringWidgets/SerieTypesComboBoxesWidget.h"
#include "Models/SimulationRatingTableModel.h"
#include "../../../seasons/SimulationSave.h"

namespace Ui {
class SimulationRatingsWindow;
}

class SimulationRatingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationRatingsWindow(SimulationSave * save, QWidget *parent = nullptr);
    ~SimulationRatingsWindow();

    void fillWindow();
    void setupConnections();

private:
    Ui::SimulationRatingsWindow *ui;
    SimulationSave * save;

    ClassificationsCheckBoxesWidget * classificationsCheckBoxes;
    CompetitionsRangeComboBoxesWidget * rangeComboBoxes;
    SerieTypesComboBoxesWidget * serieTypesCheckBoxes;

    QHash<Jumper *, QVector<CompetitionSingleResult *>> jumpersSingleResults;

    //Models:
    SimulationRatingTableModel * averagePositionModel;
    SimulationRatingTableModel * averageJudgesModel;
    SimulationRatingTableModel * averageWindModel;
    SimulationRatingTableModel * averageTakeoffRatingModel;
    SimulationRatingTableModel * averageFlightRatingModel;
    SimulationRatingTableModel * averageDistanceModel;
public:
    ClassificationsCheckBoxesWidget *getClassificationsCheckBoxes() const;
    CompetitionsRangeComboBoxesWidget *getRangeComboBoxes() const;
    SerieTypesComboBoxesWidget *getSerieTypesCheckBoxes() const;
};

#endif // SIMULATIONRATINGSWINDOW_H
