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
    classificationsCheckBoxes = new ClassificationsCheckBoxesWidget(this);
    ui->verticalLayout_classificationsComboBoxes->addWidget(classificationsCheckBoxes);
    rangeComboBoxes = new CompetitionsRangeComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(rangeComboBoxes);
    serieTypesCheckBoxes = new SerieTypesComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(serieTypesCheckBoxes);

    averagePositionModel = new SimulationRatingTableModel(this);
    ui->tableView_avgPosition->setModel(averagePositionModel);
    //QSortFilterProxyModel * proxyModel = new QSortFilterProxyModel(this);
    //proxyModel->setSourceModel(averagePositionModel);
    //ui->tableView_avgPosition->setModel(proxyModel);

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
    if(p1 == p2)
        return p1.first->getID() < p2.first->getID();
    return p1.second < p2.second;
}

bool rankingComp(const QPair<Jumper *, double> p1, const QPair<Jumper *, double> p2)
{
    if(p1 == p2)
        return p1.first->getID() > p2.first->getID();
    return p1.second > p2.second;
}

void SimulationRatingsWindow::fillWindow()
{
    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    jumpersSingleResults = CompetitionSingleResult::getJumpersFilteredSingleResults(save->getJumpersReference(), competitions,
                                                                      serieTypesCheckBoxes->getSerieTypes(), classificationsCheckBoxes->getClassifications(), classificationsCheckBoxes->allUnchecked());

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
                    avgRating += jump.getSimulationData().getTakeoffRating();
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
                    avgRating += jump.getSimulationData().getFlightRating();
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
}

void SimulationRatingsWindow::setupConnections()
{
    connect(rangeComboBoxes, &CompetitionsRangeComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(serieTypesCheckBoxes, &SerieTypesComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(classificationsCheckBoxes, &ClassificationsCheckBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
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
