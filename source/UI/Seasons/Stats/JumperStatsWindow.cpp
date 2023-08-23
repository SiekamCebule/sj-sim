#include "JumperStatsWindow.h"
#include "ui_JumperStatsWindow.h"
#include "../../../global/CountryFlagsManager.h"
#include "../../../utilities/functions.h"

JumperStatsWindow::JumperStatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumperStatsWindow)
{
    ui->setupUi(this);

    rangeComboBoxes = new CompetitionsRangeComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(rangeComboBoxes);

    serieTypesCheckBoxes = new SerieTypesComboBoxesWidget(this);
    ui->verticalLayout_serieTypesCheckBoxes->addWidget(serieTypesCheckBoxes);

    classificationsCheckBoxes = new ClassificationsCheckBoxesWidget(this);
    ui->verticalLayout_classificationsComboBoxes->addWidget(classificationsCheckBoxes);

    /*layout_takeoffRatingChart = new QVBoxLayout();
    ui->verticalLayout_charts->addItem(layout_takeoffRatingChart);
    layout_flightRatingChart = new QVBoxLayout();
    ui->verticalLayout_charts->layout()->addItem(layout_flightRatingChart);*/

    setupCharts();
}

JumperStatsWindow::~JumperStatsWindow()
{
    delete ui;
}

void JumperStatsWindow::setupConnections()
{
    connect(rangeComboBoxes, &CompetitionsRangeComboBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
    connect(serieTypesCheckBoxes, &SerieTypesComboBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
    connect(classificationsCheckBoxes, &ClassificationsCheckBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
}

void JumperStatsWindow::fillWindow()
{
    ui->label_jumperNameAndSurname->setText(jumper->getNameAndSurname());
    ui->label_jumperFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_jumperFlag->size()));

    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    QVector<CompetitionSingleResult *> singleResults = CompetitionSingleResult::getFilteredSingleResults(competitions, jumper,
        serieTypesCheckBoxes->getSerieTypes(), classificationsCheckBoxes->getClassifications(), classificationsCheckBoxes->allUnchecked());

    fillJumperApperancesChart(singleResults);
    fillJudgesPointsChart(singleResults);
    fillJumperFormChart(singleResults);
    fillTakeoffRatingChart(singleResults);
    fillFlightRatingChart(singleResults);
}

void JumperStatsWindow::fillJumperApperancesChart(QVector<CompetitionSingleResult *> & singleResults)
{
    jumperApperancesLineSeries->clear();
    int bestPosition = 100000000;
    int worstPosition = 1;
    double averagePosition = 0;
    QVector<int> positions;
    int i=1;
    for(auto & result : singleResults)
    {
        jumperApperancesLineSeries->append(i, result->getPosition());

        if(result->getPosition() < bestPosition)
            bestPosition = result->getPosition();
        else if(result->getPosition() > worstPosition)
            worstPosition = result->getPosition();
        averagePosition += result->getPosition();
        positions.push_back(result->getPosition());
        i++;
    }
    averagePosition /= singleResults.count();
    double standardDev = MyFunctions::standardDeviation(positions, averagePosition);
    standardDev = roundDoubleToTwoPlaces(standardDev);
    averagePosition = roundDoubleToTwoPlaces(averagePosition);

    jumperApperancesChart->addSeries(jumperApperancesLineSeries);
    jumperApperancesChart->createDefaultAxes();
    jumperApperancesChart->axes(Qt::Vertical).first()->setReverse(true);
    jumperApperancesChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    jumperApperancesChartView->setChart(jumperApperancesChart);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);

    ui->label_bestPos->setText(QString::number(bestPosition));
    ui->label_worstPos->setText(QString::number(worstPosition));
    ui->label_averagePos->setText(QString::number(averagePosition));
    ui->label_standardDeviation->setText(QString::number(standardDev));
}

void JumperStatsWindow::fillJudgesPointsChart(QVector<CompetitionSingleResult *> &singleResults)
{
    judgesPointsLineSeries->clear();
    QVector<double> judgesPoints;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false && singleResult->getCompetition()->getRulesPointer()->getHasJudgesPoints() == true){
                judgesPoints.push_back(jump.getJudgesPoints());
                judgesPointsLineSeries->append(i, jump.getJudgesPoints());
                i++;
            }
        }
    }
    judgesPointsChart->addSeries(judgesPointsLineSeries);
    judgesPointsChart->createDefaultAxes();
    judgesPointsChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    judgesPointsChartView->setChart(judgesPointsChart);
    judgesPointsChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillJumperFormChart(QVector<CompetitionSingleResult *> &singleResults)
{
    jumperFormLineSeries->clear();
    QVector<double> forms;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        if(singleResult->getJumpsReference().count() > 0){
            forms.push_back(singleResult->getJumpsReference().first().getJumperForm());
            jumperFormLineSeries->append(i, singleResult->getJumpsReference().first().getJumperForm());
            i++;
        }
    }
    jumperFormChart->addSeries(jumperFormLineSeries);
    jumperFormChart->createDefaultAxes();
    jumperFormChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    jumperFormChartView->setChart(jumperFormChart);
    jumperFormChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillTakeoffRatingChart(QVector<CompetitionSingleResult *> &singleResults)
{
    takeoffRatingLineSeries->clear();
    QVector<double> ratings;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false)
            {
                ratings.push_back(jump.getSimulationData().getTakeoffRating());
                takeoffRatingLineSeries->append(i, ratings.last());
                i++;
            }
        }
    }
    takeoffRatingChart->addSeries(takeoffRatingLineSeries);
    takeoffRatingChart->createDefaultAxes();
    takeoffRatingChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    takeoffRatingChartView->setChart(takeoffRatingChart);
    takeoffRatingChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillFlightRatingChart(QVector<CompetitionSingleResult *> &singleResults)
{
    flightRatingLineSeries->clear();
    QVector<double> ratings;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false)
            {
                ratings.push_back(jump.getSimulationData().getFlightRating());
                flightRatingLineSeries->append(i, ratings.last());
                i++;
            }
        }
    }
    flightRatingChart->addSeries(flightRatingLineSeries);
    flightRatingChart->createDefaultAxes();
    flightRatingChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    flightRatingChartView->setChart(flightRatingChart);
    flightRatingChartView->setRenderHint(QPainter::Antialiasing);
}

ClassificationsCheckBoxesWidget *JumperStatsWindow::getClassificationsCheckBoxes() const
{
    return classificationsCheckBoxes;
}

void JumperStatsWindow::setClassificationsCheckBoxes(ClassificationsCheckBoxesWidget *newClassificationsCheckBoxes)
{
    classificationsCheckBoxes = newClassificationsCheckBoxes;
}

void JumperStatsWindow::setupCharts()
{
    jumperApperancesChart = new QChart();
    jumperApperancesChart->legend()->hide();
    jumperApperancesChart->setTitle(tr("Występy zawodnika"));
    jumperApperancesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    jumperApperancesChartView = new QChartView(this);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_jumperApperancesChart->addWidget(jumperApperancesChartView);
    jumperApperancesLineSeries = new QLineSeries(this);

    judgesPointsChart = new QChart();
    judgesPointsChart->legend()->hide();
    judgesPointsChart->setTitle(tr("Punkty od sędziów"));
    judgesPointsChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    judgesPointsChartView = new QChartView(this);
    judgesPointsChartView->setRenderHint(QPainter::Antialiasing);
    judgesPointsChartView->setFixedHeight(450);
    ui->verticalLayout_charts->addWidget(judgesPointsChartView);
    judgesPointsLineSeries = new QLineSeries(this);

    jumperFormChart = new QChart();
    jumperFormChart->legend()->hide();
    jumperFormChart->setTitle(tr("Forma zawodnika"));
    jumperFormChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    jumperFormChartView = new QChartView(this);
    jumperFormChartView->setRenderHint(QPainter::Antialiasing);
    jumperFormChartView->setFixedHeight(450);
    ui->verticalLayout_charts->addWidget(jumperFormChartView);
    jumperFormLineSeries = new QLineSeries(this);

    takeoffRatingChart = new QChart();
    takeoffRatingChart->legend()->hide();
    takeoffRatingChart->setTitle(tr("Ocena wybicia"));
    takeoffRatingChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    takeoffRatingChartView = new QChartView(this);
    takeoffRatingChartView->setRenderHint(QPainter::Antialiasing);
    takeoffRatingChartView->setFixedHeight(450);
    ui->verticalLayout_charts->addWidget(takeoffRatingChartView);
    takeoffRatingLineSeries = new QLineSeries(this);

    flightRatingChart = new QChart();
    flightRatingChart->legend()->hide();
    flightRatingChart->setTitle(tr("Ocena lotu"));
    flightRatingChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    flightRatingChartView = new QChartView(this);
    flightRatingChartView->setRenderHint(QPainter::Antialiasing);
    flightRatingChartView->setFixedHeight(450);
    ui->verticalLayout_charts->addWidget(flightRatingChartView);
    flightRatingLineSeries = new QLineSeries(this);
}

Jumper *JumperStatsWindow::getJumper() const
{
    return jumper;
}

void JumperStatsWindow::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

CompetitionsRangeComboBoxesWidget *JumperStatsWindow::getRangeComboBoxes() const
{
    return rangeComboBoxes;
}

void JumperStatsWindow::setRangeComboBoxes(CompetitionsRangeComboBoxesWidget *newRangeComboBoxes)
{
    rangeComboBoxes = newRangeComboBoxes;
}

void JumperStatsWindow::on_horizontalScrollBar_jumperStatsChartScrollBar_valueChanged(int value)
{
    jumperApperancesChart->scroll(-value * 0.7, 0);
}

void JumperStatsWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked)
    {
        jumperFormChartView->hide();
        takeoffRatingChartView->hide();
        flightRatingChartView->hide();
    }
    else if(arg1 == Qt::Checked)
    {
        jumperFormChartView->show();
        takeoffRatingChartView->show();
        flightRatingChartView->show();
    }
}

