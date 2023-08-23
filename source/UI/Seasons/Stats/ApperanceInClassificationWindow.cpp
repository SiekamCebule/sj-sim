#include "ApperanceInClassificationWindow.h"
#include "ui_ApperanceInClassificationWindow.h"
#include "../../../global/CountryFlagsManager.h"
#include "../../../simulator/Jumper.h"
#include "../../../simulator/Team.h"
#include "../../../competitions/CompetitionInfo.h"
#include "../../../utilities/functions.h"

ApperanceInClassificationWindow::ApperanceInClassificationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApperanceInClassificationWindow),
    jumper(nullptr)
{
    ui->setupUi(this);

    setupChart();
}

ApperanceInClassificationWindow::~ApperanceInClassificationWindow()
{
    delete ui;
}

void ApperanceInClassificationWindow::fillWindow()
{
    if(jumper != nullptr)
    {
        ui->label_jumperNameAndSurname->show();
        ui->label_jumperNameAndSurname->setText(jumper->getNameAndSurname());
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower()))
                                      .scaled(ui->label_flag->size()));
    }
    else if(teamCode != "")
    {
        ui->label_jumperNameAndSurname->hide();
        ui->label_flag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(teamCode.toLower()))
                                      .scaled(ui->label_flag->size()));
    }
    fillChart();
    ui->label_classification->setText(classification->getName());

}

void ApperanceInClassificationWindow::setupChart()
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle(tr("Miejsca w klasyfikacji"));
    chart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_chart->addWidget(chartView);
    series = new QLineSeries(this);
}

void ApperanceInClassificationWindow::fillChart()
{
    series->clear();

    //Mamy w archiveResults pozycje Kamila Stocha w klasyfikacji po każdym konkursie
    //Chcemy na wykresie pokazać pozycje po koleji (od najstarszego konkursu)
    QVector<CompetitionInfo* > archiveResultsCompetitions = archiveResults.keys();
    QVector<int> positions;
    for(auto & comp : season->getCalendarReference().getCompetitionsReference())
    {
        if(archiveResultsCompetitions.contains(comp))
            positions.push_back(archiveResults.value(comp));
    }

    int i=1;
    for(auto & position : positions)
    {
        series->append(i, position);
        i++;
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setReverse(true);
    chart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    int bestPos = 100000000;
    int worstPos = 1;
    double avgPos = 0;
    for(auto & pos : positions)
    {
        if(pos < bestPos)
            bestPos = pos;
        else if(pos > worstPos)
            worstPos = pos;
        avgPos += pos;
    }
    avgPos /= positions.count();
    avgPos = roundDoubleToTwoPlaces(avgPos);
    double standardDeviation = roundDoubleToOnePlace(MyFunctions::standardDeviation(positions, avgPos));

    ui->label_bestPos->setText(QString::number(bestPos));
    ui->label_worstPos->setText(QString::number(worstPos));
    ui->label_averagePos->setText(QString::number(avgPos));
    ui->label_standardDeviation->setText(QString::number(standardDeviation));
}

Season *ApperanceInClassificationWindow::getSeason() const
{
    return season;
}

void ApperanceInClassificationWindow::setSeason(Season *newSeason)
{
    season = newSeason;
}

QHash<CompetitionInfo *, int> ApperanceInClassificationWindow::getArchiveResults() const
{
    return archiveResults;
}

void ApperanceInClassificationWindow::setArchiveResults(const QHash<CompetitionInfo *, int> &newArchiveResults)
{
    archiveResults = newArchiveResults;
}

Classification *ApperanceInClassificationWindow::getClassification() const
{
    return classification;
}

void ApperanceInClassificationWindow::setClassification(Classification *newClassification)
{
    classification = newClassification;
}

QString ApperanceInClassificationWindow::getTeamCode() const
{
    return teamCode;
}

void ApperanceInClassificationWindow::setTeamCode(const QString &newTeamCode)
{
    teamCode = newTeamCode;
}

Jumper *ApperanceInClassificationWindow::getJumper() const
{
    return jumper;
}

void ApperanceInClassificationWindow::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}
