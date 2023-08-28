#include "SimulationRatingsWindow.h"
#include "ui_SimulationRatingsWindow.h"
#include "../../../utilities/functions.h"
#include <QSortFilterProxyModel>

SimulationRatingsWindow::SimulationRatingsWindow(SimulationSave * save, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationRatingsWindow),
    save(save)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    classificationsCheckBoxes = new ClassificationsCheckBoxesWidget(this);
    ui->verticalLayout_classificationsComboBoxes->addWidget(classificationsCheckBoxes);
    rangeComboBoxes = new CompetitionsRangeComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(rangeComboBoxes);
    serieTypesCheckBoxes = new SerieTypesComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(serieTypesCheckBoxes);
    hillTypesCheckBoxes = new HillTypesCheckBoxesWidget(this);
    ui->verticalLayout_hillTypesCheckBoxes->addWidget(hillTypesCheckBoxes);

    //Averages models
    averagePositionModel = new SimulationRatingTableModel(this);
    ui->tableView_avgPosition->setModel(averagePositionModel);
    averageJudgesModel = new SimulationRatingTableModel(this);
    ui->tableView_avgJudges->setModel(averagePositionModel);
    averageWindModel = new SimulationRatingTableModel(this);
    ui->tableView_avgWind->setModel(averagePositionModel);
    averageTakeoffRatingModel = new SimulationRatingTableModel(this);
    ui->tableView_avgTakeoffRating->setModel(averagePositionModel);
    averageFlightRatingModel = new SimulationRatingTableModel(this);
    ui->tableView_avgFlightRating->setModel(averagePositionModel);
    averageDistanceModel = new SimulationRatingTableModel(this);
    ui->tableView_avgDistance->setModel(averagePositionModel);
    //Records models
    bestDistanceModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestDistance->setModel(bestDistanceModel);
    worstDistanceModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstDistance->setModel(worstDistanceModel);
    bestPointsModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestPoints->setModel(bestPointsModel);
    worstPointsModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstPoints->setModel(worstPointsModel);
    bestJudgesModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestJudges->setModel(bestJudgesModel);
    worstJudgesModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstJudges->setModel(worstJudgesModel);
    bestWindModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestWind->setModel(bestWindModel);
    worstWindModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstWind->setModel(worstWindModel);
    bestTakeoffModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestTakeoff->setModel(bestTakeoffModel);
    worstTakeoffModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstTakeoff->setModel(worstTakeoffModel);
    bestFlightModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestFlight->setModel(bestFlightModel);
    worstFlightModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstFlight->setModel(worstFlightModel);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index){
        if(index == 0)
        {
            ui->spinBox_recordsCount->hide();
            ui->label_recordsCount->hide();
        }
        else if(index == 1)
        {
            ui->spinBox_recordsCount->show();
            ui->label_recordsCount->show();
        }
    });
    emit ui->tabWidget->currentChanged(0);
    connect(ui->spinBox_recordsCount, &QSpinBox::editingFinished, this, &SimulationRatingsWindow::fillWindow);

    connect(ui->checkBox_showHidden, &QCheckBox::stateChanged, this, [this](int state){
        if(state == 0)
        {
            ui->label_takeoff->hide();
            ui->label_flight->hide();
            ui->label_avgTakeoff->hide();
            ui->label_avgFlight->hide();
            ui->tableView_avgTakeoffRating->hide();
            ui->tableView_avgFlightRating->hide();
            ui->tableView_bestTakeoff->hide();
            ui->tableView_bestFlight->hide();
            ui->tableView_worstTakeoff->hide();
            ui->tableView_worstFlight->hide();
        }
        else if(state == 2)
        {
            ui->label_takeoff->show();
            ui->label_flight->show();
            ui->label_avgTakeoff->show();
            ui->label_avgFlight->show();
            ui->tableView_avgTakeoffRating->show();
            ui->tableView_avgFlightRating->show();
            ui->tableView_bestTakeoff->show();
            ui->tableView_bestFlight->show();
            ui->tableView_worstTakeoff->show();
            ui->tableView_worstFlight->show();
        }
    });
    emit ui->checkBox_showHidden->stateChanged(0);
}

SimulationRatingsWindow::~SimulationRatingsWindow()
{
    delete ui;
}

bool rankingCompForPositions(const QPair<Jumper *, double> p1, const QPair<Jumper *, double> p2)
{
    if(p1.second == 0)
        return false;
    else if(p2.second == 0)
        return true;
    if(p1.second == p2.second)
        return p1.first->getID() < p2.first->getID();
    return p1.second < p2.second;
}

bool rankingComp(const QPair<Jumper *, double> p1, const QPair<Jumper *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() > p2.first->getID();
    return p1.second > p2.second;
}

bool descendingCompForRecords(const QPair<JumpData *, double> p1, const QPair<JumpData *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() > p2.first->getID();
    return p1.second > p2.second;
}
bool ascendingCompForRecords(const QPair<JumpData *, double> p1, const QPair<JumpData *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() < p2.first->getID();
    return p1.second < p2.second;
}

void SimulationRatingsWindow::fillWindow()
{
    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    jumpersSingleResults = CompetitionSingleResult::getJumpersFilteredSingleResults(save->getJumpersReference(), competitions,
                                                                                    serieTypesCheckBoxes->getSerieTypes(), hillTypesCheckBoxes->getHillTypesSet(), classificationsCheckBoxes->getClassifications(), classificationsCheckBoxes->allUnchecked());

    //Średnia pozycja
    QVector<QPair<Jumper *, double>> avgPositionsRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        double avgPosition = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            avgPosition += result->getPosition();
        }
        avgPosition /= jumpersSingleResults.value(jumper).count();
        avgPosition = roundDoubleToTwoPlaces(avgPosition);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgPosition = 0;
        avgPositionsRanking.push_back(qMakePair(jumper, avgPosition));
    }
    std::sort(avgPositionsRanking.begin(), avgPositionsRanking.end(), &rankingCompForPositions);
    averagePositionModel->setRanking(avgPositionsRanking);
    ui->tableView_avgPosition->setModel(nullptr);
    ui->tableView_avgPosition->setModel(averagePositionModel);
    ui->tableView_avgPosition->resizeColumnsToContents();

    //Średnie noty za styl
    QVector<QPair<Jumper *, double>> avgJudgesRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgJudges = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false && result->getCompetition()->getRulesPointer()->getHasJudgesPoints())
                {
                    for(auto & jg : jump.getJudges())
                    {
                        avgJudges += jg;
                        count++;
                    }
                }
            }
        }
        avgJudges /= count;
        avgJudges = roundDoubleToTwoPlaces(avgJudges);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgJudges = 0;
        avgJudgesRanking.push_back(qMakePair(jumper, avgJudges));
    }
    std::sort(avgJudgesRanking.begin(), avgJudgesRanking.end(), &rankingComp);
    averageJudgesModel->setRanking(avgJudgesRanking);
    ui->tableView_avgJudges->setModel(nullptr);
    ui->tableView_avgJudges->setModel(averageJudgesModel);
    ui->tableView_avgJudges->resizeColumnsToContents();

    //Średni wiatr
    QVector<QPair<Jumper *, double>> avgWindRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgWind = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false && result->getCompetition()->getRulesPointer()->getHasWindCompensations())
                {
                    avgWind += jump.getAveragedWind();
                    count++;
                }
            }
        }
        avgWind /= count;
        avgWind = roundDoubleToTwoPlaces(avgWind);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgWind = 0;
        avgWindRanking.push_back(qMakePair(jumper, avgWind));
    }
    std::sort(avgWindRanking.begin(), avgWindRanking.end(), &rankingComp);
    averageWindModel->setRanking(avgWindRanking);
    ui->tableView_avgWind->setModel(nullptr);
    ui->tableView_avgWind->setModel(averageWindModel);
    ui->tableView_avgWind->resizeColumnsToContents();

    //Ocena wybicia
    QVector<QPair<Jumper *, double>> avgTakeoffRatingRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgRating = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgRating += jump.getSimulationDataReference().getTakeoffRating();
                    count++;
                }
            }
        }
        avgRating /= count;
        avgRating = roundDoubleToTwoPlaces(avgRating);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgRating = 0;
        avgTakeoffRatingRanking.push_back(qMakePair(jumper, avgRating));
    }
    std::sort(avgTakeoffRatingRanking.begin(), avgTakeoffRatingRanking.end(), &rankingComp);
    averageTakeoffRatingModel->setRanking(avgTakeoffRatingRanking);
    ui->tableView_avgTakeoffRating->setModel(nullptr);
    ui->tableView_avgTakeoffRating->setModel(averageTakeoffRatingModel);
    ui->tableView_avgTakeoffRating->resizeColumnsToContents();

    //Ocena lotu
    QVector<QPair<Jumper *, double>> avgFlightRatingRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgRating = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgRating += jump.getSimulationDataReference().getFlightRating();
                    count++;
                }
            }
        }
        avgRating /= count;
        avgRating = roundDoubleToTwoPlaces(avgRating);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgRating = 0;
        avgFlightRatingRanking.push_back(qMakePair(jumper, avgRating));
    }
    std::sort(avgFlightRatingRanking.begin(), avgFlightRatingRanking.end(), &rankingComp);
    averageFlightRatingModel->setRanking(avgFlightRatingRanking);
    ui->tableView_avgFlightRating->setModel(nullptr);
    ui->tableView_avgFlightRating->setModel(averageFlightRatingModel);
    ui->tableView_avgFlightRating->resizeColumnsToContents();

    //Odległosść
    QVector<QPair<Jumper *, double>> avgDistanceRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgDistance = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgDistance += jump.getDistance();
                    count++;
                }
            }
        }
        avgDistance /= count;
        avgDistance = roundDoubleToTwoPlaces(avgDistance);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgDistance = 0;
        avgDistanceRanking.push_back(qMakePair(jumper, avgDistance));
    }
    std::sort(avgDistanceRanking.begin(), avgDistanceRanking.end(), &rankingComp);
    averageDistanceModel->setRanking(avgDistanceRanking);
    ui->tableView_avgDistance->setModel(nullptr);
    ui->tableView_avgDistance->setModel(averageDistanceModel);
    ui->tableView_avgDistance->resizeColumnsToContents();

    //-------------------REKORDY---------------------
    //Odległość
    //NAJLEPSZA
    int recordsMaxSize = ui->spinBox_recordsCount->value();
    QVector<QPair<JumpData *, double>> bestDistanceRecords;
    QVector<QPair<JumpData *, double>> worstDistanceRecords;
    QVector<QPair<JumpData *, double>> bestPointsRecords;
    QVector<QPair<JumpData *, double>> worstPointsRecords;
    QVector<QPair<JumpData *, double>> bestJudgesRecords;
    QVector<QPair<JumpData *, double>> worstJudgesRecords;
    QVector<QPair<JumpData *, double>> bestWindRecords;
    QVector<QPair<JumpData *, double>> worstWindRecords;
    QVector<QPair<JumpData *, double>> bestTakeoffRecords;
    QVector<QPair<JumpData *, double>> worstTakeoffRecords;
    QVector<QPair<JumpData *, double>> bestFlightRecords;
    QVector<QPair<JumpData *, double>> worstFlightRecords;

    for(auto & jumper : jumpersSingleResults.keys())
        for(auto & result : jumpersSingleResults.value(jumper))
            for(auto & jump : result->getJumpsReference())
                if(jump.getDSQ() == false)
                {
                    //----------ODLEGŁOŚĆ----------
                    //najlepsze
                    double val = jump.getDistance();
                    if(bestDistanceRecords.size() < recordsMaxSize)
                        bestDistanceRecords.push_back(qMakePair(&jump, val));
                    else if(val > bestDistanceRecords.last().second)
                        bestDistanceRecords[bestDistanceRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestDistanceRecords.begin(), bestDistanceRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstDistanceRecords.size() < recordsMaxSize)
                        worstDistanceRecords.push_back(qMakePair(&jump, val));
                    else if(val < worstDistanceRecords.last().second)
                        worstDistanceRecords[worstDistanceRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstDistanceRecords.begin(), worstDistanceRecords.end(), &ascendingCompForRecords);

                    //----------PUNKTY----------
                    //najlepsze
                    val = jump.getPoints();
                    if(bestPointsRecords.size() < recordsMaxSize)
                        bestPointsRecords.push_back(qMakePair(&jump, val));
                    else if(val > bestPointsRecords.last().second)
                        bestPointsRecords[bestPointsRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestPointsRecords.begin(), bestPointsRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstPointsRecords.size() < recordsMaxSize)
                        worstPointsRecords.push_back(qMakePair(&jump, val));
                    else if(val < worstPointsRecords.last().second)
                        worstPointsRecords[worstPointsRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstPointsRecords.begin(), worstPointsRecords.end(), &ascendingCompForRecords);


                    //----------PUNKTY OD SĘDZIÓW----------
                    //najlepsze
                    if(jump.getCompetition()->getRulesPointer()->getHasJudgesPoints()){
                        val = jump.getJudgesPoints();
                        if(bestJudgesRecords.size() < recordsMaxSize)
                            bestJudgesRecords.push_back(qMakePair(&jump, val));
                        else if(val > bestJudgesRecords.last().second)
                            bestJudgesRecords[bestJudgesRecords.count() - 1] = qMakePair(&jump, val);
                        std::sort(bestJudgesRecords.begin(), bestJudgesRecords.end(), &descendingCompForRecords);
                        //najgorsze
                        if(worstJudgesRecords.size() < recordsMaxSize)
                            worstJudgesRecords.push_back(qMakePair(&jump, val));
                        else if(val < worstJudgesRecords.last().second)
                            worstJudgesRecords[worstJudgesRecords.count() - 1] = qMakePair(&jump, val);
                        std::sort(worstJudgesRecords.begin(), worstJudgesRecords.end(), &ascendingCompForRecords);
                    }

                    //----------UŚREDNIONY WIATR----------
                    //najlepsze
                    val = jump.getAveragedWind();
                    if(bestWindRecords.size() < recordsMaxSize)
                        bestWindRecords.push_back(qMakePair(&jump, val));
                    else if(val > bestWindRecords.last().second)
                        bestWindRecords[bestWindRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestWindRecords.begin(), bestWindRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstWindRecords.size() < recordsMaxSize)
                        worstWindRecords.push_back(qMakePair(&jump, val));
                    else if(val < worstWindRecords.last().second)
                        worstWindRecords[worstWindRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstWindRecords.begin(), worstWindRecords.end(), &ascendingCompForRecords);

                    //----------OCENA WYBICIA----------
                    //najlepsze
                    val = jump.getSimulationDataReference().getTakeoffRating();
                    if(bestTakeoffRecords.size() < recordsMaxSize)
                        bestTakeoffRecords.push_back(qMakePair(&jump, val));
                    else if(val > bestTakeoffRecords.last().second)
                        bestTakeoffRecords[bestTakeoffRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestTakeoffRecords.begin(), bestTakeoffRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstTakeoffRecords.size() < recordsMaxSize)
                        worstTakeoffRecords.push_back(qMakePair(&jump, val));
                    else if(val < worstTakeoffRecords.last().second)
                        worstTakeoffRecords[worstTakeoffRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstTakeoffRecords.begin(), worstTakeoffRecords.end(), &ascendingCompForRecords);

                    //----------OCENA LOTU----------
                    //najlepsze
                    val = jump.getSimulationDataReference().getFlightRating();
                    if(bestFlightRecords.size() < recordsMaxSize)
                        bestFlightRecords.push_back(qMakePair(&jump, val));
                    else if(val > bestFlightRecords.last().second)
                        bestFlightRecords[bestFlightRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestFlightRecords.begin(), bestFlightRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstFlightRecords.size() < recordsMaxSize)
                        worstFlightRecords.push_back(qMakePair(&jump, val));
                    else if(val < worstFlightRecords.last().second)
                        worstFlightRecords[worstFlightRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstFlightRecords.begin(), worstFlightRecords.end(), &ascendingCompForRecords);
                }


    bestDistanceModel->setRecords(bestDistanceRecords);
    ui->tableView_bestDistance->setModel(nullptr);
    ui->tableView_bestDistance->setModel(bestDistanceModel);
    ui->tableView_bestDistance->resizeColumnsToContents();
    worstDistanceModel->setRecords(worstDistanceRecords);
    ui->tableView_worstDistance->setModel(nullptr);
    ui->tableView_worstDistance->setModel(worstDistanceModel);
    ui->tableView_worstDistance->resizeColumnsToContents();

    bestPointsModel->setRecords(bestPointsRecords);
    ui->tableView_bestPoints->setModel(nullptr);
    ui->tableView_bestPoints->setModel(bestPointsModel);
    ui->tableView_bestPoints->resizeColumnsToContents();
    worstPointsModel->setRecords(worstPointsRecords);
    ui->tableView_worstPoints->setModel(nullptr);
    ui->tableView_worstPoints->setModel(worstPointsModel);
    ui->tableView_worstPoints->resizeColumnsToContents();

    bestJudgesModel->setRecords(bestJudgesRecords);
    ui->tableView_bestJudges->setModel(nullptr);
    ui->tableView_bestJudges->setModel(bestJudgesModel);
    ui->tableView_bestJudges->resizeColumnsToContents();
    worstJudgesModel->setRecords(worstJudgesRecords);
    ui->tableView_worstJudges->setModel(nullptr);
    ui->tableView_worstJudges->setModel(worstJudgesModel);
    ui->tableView_worstJudges->resizeColumnsToContents();

    bestWindModel->setRecords(bestWindRecords);
    ui->tableView_bestWind->setModel(nullptr);
    ui->tableView_bestWind->setModel(bestWindModel);
    ui->tableView_bestWind->resizeColumnsToContents();
    worstWindModel->setRecords(worstWindRecords);
    ui->tableView_worstWind->setModel(nullptr);
    ui->tableView_worstWind->setModel(worstWindModel);
    ui->tableView_worstWind->resizeColumnsToContents();

    bestTakeoffModel->setRecords(bestTakeoffRecords);
    ui->tableView_bestTakeoff->setModel(nullptr);
    ui->tableView_bestTakeoff->setModel(bestTakeoffModel);
    ui->tableView_bestTakeoff->resizeColumnsToContents();
    worstTakeoffModel->setRecords(worstTakeoffRecords);
    ui->tableView_worstTakeoff->setModel(nullptr);
    ui->tableView_worstTakeoff->setModel(worstTakeoffModel);
    ui->tableView_worstTakeoff->resizeColumnsToContents();

    bestFlightModel->setRecords(bestFlightRecords);
    ui->tableView_bestFlight->setModel(nullptr);
    ui->tableView_bestFlight->setModel(bestFlightModel);
    ui->tableView_bestFlight->resizeColumnsToContents();
    worstFlightModel->setRecords(worstFlightRecords);
    ui->tableView_worstFlight->setModel(nullptr);
    ui->tableView_worstFlight->setModel(worstFlightModel);
    ui->tableView_worstFlight->resizeColumnsToContents();
}

void SimulationRatingsWindow::setupConnections()
{
    connect(rangeComboBoxes, &CompetitionsRangeComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(serieTypesCheckBoxes, &SerieTypesComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(classificationsCheckBoxes, &ClassificationsCheckBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(hillTypesCheckBoxes, &HillTypesCheckBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
}

HillTypesCheckBoxesWidget *SimulationRatingsWindow::getHillTypesCheckBoxes() const
{
    return hillTypesCheckBoxes;
}

SerieTypesComboBoxesWidget *SimulationRatingsWindow::getSerieTypesCheckBoxes() const
{
    return serieTypesCheckBoxes;
}

CompetitionsRangeComboBoxesWidget *SimulationRatingsWindow::getRangeComboBoxes() const
{
    return rangeComboBoxes;
}

ClassificationsCheckBoxesWidget *SimulationRatingsWindow::getClassificationsCheckBoxes() const
{
    return classificationsCheckBoxes;
}