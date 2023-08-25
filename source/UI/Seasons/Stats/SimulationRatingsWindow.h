#ifndef SIMULATIONRATINGSWINDOW_H
#define SIMULATIONRATINGSWINDOW_H

#include <QDialog>
#include "FilteringWidgets/ClassificationsCheckBoxesWidget.h"
#include "FilteringWidgets/CompetitionsRangeComboBoxesWidget.h"
#include "FilteringWidgets/SerieTypesComboBoxesWidget.h"
#include "FilteringWidgets/HillTypesCheckBoxesWidget.h"
#include "Models/SimulationRatingTableModel.h"
#include "Models/SimulationRecordsTableModel.h"
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
    HillTypesCheckBoxesWidget * hillTypesCheckBoxes;

    QHash<Jumper *, QVector<CompetitionSingleResult *>> jumpersSingleResults;

    //Averages models:
    SimulationRatingTableModel * averagePositionModel;
    SimulationRatingTableModel * averageJudgesModel;
    SimulationRatingTableModel * averageWindModel;
    SimulationRatingTableModel * averageTakeoffRatingModel;
    SimulationRatingTableModel * averageFlightRatingModel;
    SimulationRatingTableModel * averageDistanceModel;
    //Records models:
    SimulationRecordsTableModel * bestDistanceModel;
    SimulationRecordsTableModel * worstDistanceModel;
    SimulationRecordsTableModel * bestPointsModel;
    SimulationRecordsTableModel * worstPointsModel;
    SimulationRecordsTableModel * bestJudgesModel;
    SimulationRecordsTableModel * worstJudgesModel;
    SimulationRecordsTableModel * bestWindModel;
    SimulationRecordsTableModel * worstWindModel;
    SimulationRecordsTableModel * bestTakeoffModel;
    SimulationRecordsTableModel * worstTakeoffModel;
    SimulationRecordsTableModel * bestFlightModel;
    SimulationRecordsTableModel * worstFlightModel;

public:
    ClassificationsCheckBoxesWidget *getClassificationsCheckBoxes() const;
    CompetitionsRangeComboBoxesWidget *getRangeComboBoxes() const;
    SerieTypesComboBoxesWidget *getSerieTypesCheckBoxes() const;
    HillTypesCheckBoxesWidget *getHillTypesCheckBoxes() const;
};

#endif // SIMULATIONRATINGSWINDOW_H
