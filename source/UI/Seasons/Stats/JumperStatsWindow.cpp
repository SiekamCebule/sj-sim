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
}

void JumperStatsWindow::fillWindow()
{
    ui->label_jumperNameAndSurname->setText(jumper->getNameAndSurname());
    ui->label_jumperFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_jumperFlag->size()));

    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    QVector<CompetitionSingleResult *> singleResults = CompetitionSingleResult::getFilteredSingleResults(competitions, jumper, serieTypesCheckBoxes->getSerieTypes());
    fillJumperApperancesChart(singleResults);
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
    //jumperApperancesChart->axes(Qt::Horizontal).first()->setRange(0, 30);

    jumperApperancesChartView->setChart(jumperApperancesChart);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);

    ui->label_bestPos->setText(QString::number(bestPosition));
    ui->label_worstPos->setText(QString::number(worstPosition));
    ui->label_averagePos->setText(QString::number(averagePosition));
    ui->label_standardDeviation->setText(QString::number(standardDev));
}

void JumperStatsWindow::setupCharts()
{
    jumperApperancesChart = new QChart();
    jumperApperancesChart->legend()->hide();
    jumperApperancesChart->setTitle("Występy zawodnika");
    jumperApperancesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));

    jumperApperancesChartView = new QChartView(this);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);
    ui->verticalLayout_jumperApperancesChart->addWidget(jumperApperancesChartView);

    jumperApperancesLineSeries = new QLineSeries(this);
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
