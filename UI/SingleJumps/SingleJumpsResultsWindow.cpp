#include "SingleJumpsResultsWindow.h"
#include "ui_SingleJumpsResultsWindow.h"

#include "SingleJumpMiniResultWidget.h"
#include "../ResultsShowing/JumpDataDetailedInfoWindow.h"
#include "../../global/CountryFlagsManager.h"
#include "../../global/IDGenerator.h"

#include <math.h>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QEventLoop>
#include <QMap>
#include <QDebug>
#include <QAction>
#include <QInputDialog>

extern IDGenerator globalIDGenerator;

SingleJumpsResultsWindow::SingleJumpsResultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsResultsWindow),
    maxNumberOfDistancesForChart(0),
    maxNumberOfJudgesForChart(0),
    maxNumberOfLandingsForChart(0),
    selectedItemIndex(0),
    maxNumberOfWindsForChart(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

SingleJumpsResultsWindow::~SingleJumpsResultsWindow()
{
    qDebug()<<ui;
    delete ui;
    qDebug()<<"usunieto ui2";
}

void SingleJumpsResultsWindow::fillJumperInfo()
{
    ui->label_jumperNameAndSurname->setText(manager->getJumper().getNameAndSurname());
    ui->label_jumperFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(manager->getJumper().getCountryCode().toLower())).scaled(ui->label_jumperFlag->size()));
}

void SingleJumpsResultsWindow::fillHillInfo()
{
    ui->label_hillName->setText(manager->getHill().getName() + " HS" + QString::number(manager->getHill().getHSPoint()));
    ui->label_hillFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(manager->getHill().getCountryCode().toLower())).scaled(ui->label_hillFlag->size()));
}

void SingleJumpsResultsWindow::fillMiniJumpsResultsLayout()
{
    //PROBLEM: W wywołaniu slotu JumpData pozostaje prawidłowa, lecz składowe tej JumpDaty psują się (wszystkie)
    int i=1;
    for(auto & jump : manager->getEditableJumps())
    {
        SingleJumpMiniResultWidget * widget = new SingleJumpMiniResultWidget;
        widget->setJumpData(const_cast<JumpData*>(&jump));
        widget->setIndexInList(i);
        widget->fillUi();

        ui->verticalLayout_singleJumpResultWidgets->addWidget(widget);
        miniResultItems.push_back(widget);

        connect(widget, &SingleJumpMiniResultWidget::pressed, this, [this, widget, jump](){
            if(ui->verticalLayout_jumpInfos != nullptr){
                QLayoutItem * item;
                while((item = ui->verticalLayout_jumpInfos->takeAt(0)) != NULL)
                {
                    delete item->widget();
                    delete item;
                }
            }
            JumpDataDetailedInfoWindow * jumpInfo = new JumpDataDetailedInfoWindow(widget->getJumpData());
            jumpInfo->fillJumpInformations();
            ui->verticalLayout_jumpInfos->addWidget(jumpInfo);
            ui->toolBox->setCurrentIndex(0);

            setSelectedItemIndex(widget->getIndexInList());
            widget->setIsSelected(true);
        });

        i++;
    }
    ui->verticalLayout_singleJumpResultWidgets->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void SingleJumpsResultsWindow::fillDistancesChart()
{
    qDebug()<<"in chart";
    QChart * distancesChart = new QChart();
    qDebug()<<"in chart";
    distancesChart->legend()->hide();
    qDebug()<<"in chart";
    distancesChart->addSeries(getSplineSeriesForDistancesChart());
    qDebug()<<"in chart";
    distancesChart->setTitle("Rozkład odległości zawodnika");
    qDebug()<<"in chart";
    distancesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    qDebug()<<"in chart";
    distancesChart->createDefaultAxes();
    qDebug()<<"in chart";
    distancesChart->axes(Qt::Vertical).first()->setRange(1, getMaxNumberOfDistancesForChart() * 1.15);
    qDebug()<<"in chart";

    QChartView * distancesChartView = new QChartView(distancesChart);
    qDebug()<<"in chart";
    distancesChartView->setRenderHint(QPainter::Antialiasing);
    qDebug()<<"in chart";

    ui->verticalLayout_distanceStatistics->addWidget(distancesChartView);
    qDebug()<<"in chart";
}

void SingleJumpsResultsWindow::fillJudgesChart()
{
    QChart * judgesChart = new QChart();
    judgesChart->legend()->hide();
    judgesChart->addSeries(getSplineSeriesForJudgesChart());
    judgesChart->setTitle("Rozkład not sędziowskich zawodnika");
    judgesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    judgesChart->createDefaultAxes();
    judgesChart->axes(Qt::Vertical).first()->setRange(1, maxNumberOfJudgesForChart * 1.15);

    QChartView * judgesChartView = new QChartView(judgesChart);
    judgesChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_judgesStatistics->addWidget(judgesChartView);
}

void SingleJumpsResultsWindow::fillLandingsChart()
{
    QChart * landingsChart = new QChart();
    landingsChart->addSeries(getBarSeriesForLandingsChar());
    landingsChart->setTitle("Rozkład rodzajów lądowania zawodnika");
    landingsChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    landingsChart->createDefaultAxes();
    landingsChart->axes(Qt::Vertical).first()->setRange(0, maxNumberOfLandingsForChart * 1.15);

    QChartView * landingsChartView = new QChartView(landingsChart);
    landingsChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_landingsStatistics->addWidget(landingsChartView);
}

void SingleJumpsResultsWindow::fillWindsChart()
{
    QChart * windsChart = new QChart();
    windsChart->addSeries(getSplineSeriesForWindsChart());
    windsChart->setTitle("Rozkład siły wiatru przy skokach (zaokrąglone do 0.05)");
    windsChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    windsChart->createDefaultAxes();
    windsChart->axes(Qt::Vertical).first()->setRange(0, maxNumberOfWindsForChart * 1.15);

    QChartView * windsChartView = new QChartView(windsChart);
    windsChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_windStatistics->addWidget(windsChartView);
}

SingleJumpsManager *SingleJumpsResultsWindow::getManager() const
{
    return manager;
}

void SingleJumpsResultsWindow::setManager(SingleJumpsManager *newManager)
{
    manager = newManager;
}

QSplineSeries *SingleJumpsResultsWindow::getSplineSeriesForDistancesChart()
{
    QSplineSeries * series = new QSplineSeries();
    QMap<int, int> distances;
    for(const auto & jump : manager->getJumps())
    {
        double distance = std::round(jump.getDistance());
        if(distances.contains(distance))
        {
            distances[distance] += 1;
        }
        else{
            distances.insert(distance, 1);
        }
    }

    for(auto & key : distances.keys())
    {
        if(distances.value(key) > maxNumberOfDistancesForChart) maxNumberOfDistancesForChart = distances.value(key);
        series->append(key, distances.value(key));
    }

    return series;
}

QSplineSeries *SingleJumpsResultsWindow::getSplineSeriesForJudgesChart()
{
    QSplineSeries * series = new QSplineSeries();
    QMap<double, int> judges;
    for(const auto & jump : manager->getJumps())
    {
        for(int i=0; i<5; i++){
            double judge = jump.getJudges().at(i);//std::round(jump.getJudges().at(i));
            if(judges.contains(judge))
            {
                judges[judge] += 1;
            }
            else{
                judges[judge] = 1;
            }
        }
    }
    for(auto & key : judges.keys())
    {
        if(judges.value(key) > maxNumberOfJudgesForChart) maxNumberOfJudgesForChart = judges.value(key);
        series->append(key, judges.value(key));
    }

    return series;
}

QBarSeries *SingleJumpsResultsWindow::getBarSeriesForLandingsChar()
{
    QBarSet * telemarkSet = new QBarSet(tr("Telemark"));
    QBarSet * bothLegsSet = new QBarSet(tr("Na dwie nogi"));
    QBarSet * supportSet = new QBarSet(tr("Podpórka"));
    QBarSet * fallSet = new QBarSet(tr("Upadek"));

    QMap<short, int> landings;
    for(const auto & jump : manager->getJumps())
    {
        short type = jump.getLanding().getType();
        if(landings.contains(type))
        {
            landings[type] += 1;
        }
        else{
            landings[type] = 1;
        }
    }
    for(auto & key : landings.keys())
    {
        if(landings.value(key) > maxNumberOfLandingsForChart) maxNumberOfLandingsForChart = landings.value(key);
    }
    telemarkSet->append(landings.value(Landing::TelemarkLanding));
    bothLegsSet->append(landings.value(Landing::BothLegsLanding));
    supportSet->append(landings.value(Landing::SupportLanding));
    fallSet->append(landings.value(Landing::Fall));

    QBarSeries * series = new QBarSeries;
    series->append(telemarkSet);
    series->append(bothLegsSet);
    series->append(supportSet);
    series->append(fallSet);

    return series;
}

QSplineSeries *SingleJumpsResultsWindow::getSplineSeriesForWindsChart()
{
    QSplineSeries * series = new QSplineSeries();
    QMap<double, int> winds;
    for(const auto & jump : manager->getJumps())
    {
        double wind = jump.getAveragedWind();
        wind *= 20;
        wind = round(wind);
        wind *= 0.05;
        if(winds.contains(wind))
        {
            winds[wind] += 1;
        }
        else{
            winds.insert(wind, 1);
        }
    }
    for(auto & key : winds.keys())
    {
        if(winds.value(key) > maxNumberOfWindsForChart) maxNumberOfWindsForChart = winds.value(key);
        series->append(key, winds.value(key));
    }

    return series;
}

void SingleJumpsResultsWindow::installShortcuts()
{
    QAction * action = new QAction(this);
    action->setShortcut(Qt::CTRL | Qt::Key_I);
    connect(action, &QAction::triggered, this, &SingleJumpsResultsWindow::askForIndexForJumpInformationShow);
    this->addAction(action);
}

void SingleJumpsResultsWindow::askForIndexForJumpInformationShow()
{
    qDebug()<<"fajne";
    if(manager->getJumpsCount() > 0){
        QInputDialog dialog;
        dialog.setInputMode(QInputDialog::IntInput);
        dialog.setModal(true);
        dialog.setIntMinimum(1);
        dialog.setIntMaximum(manager->getJumpsCount());
        dialog.setStyleSheet("QWidget{background-color: rgb(225, 225, 225); color: black;}");
        dialog.setWindowTitle(tr("Przejdź do wybranej informacji o skoku"));
        dialog.setLabelText(tr("Wpisz numer skoku do którego chcesz się przenieść"));
        if(dialog.exec() == QDialog::Accepted)
        {
            int index = dialog.intValue() - 1;
            if(ui->verticalLayout_jumpInfos != nullptr){
                QLayoutItem * item;
                while((item = ui->verticalLayout_jumpInfos->takeAt(0)) != NULL)
                {
                    delete item->widget();
                    delete item;
                }
            }
            JumpDataDetailedInfoWindow * jumpInfo = new JumpDataDetailedInfoWindow(const_cast<JumpData *>(&manager->getJumps().at(index)));
            jumpInfo->fillJumpInformations();
            ui->verticalLayout_jumpInfos->addWidget(jumpInfo);
            ui->toolBox->setCurrentIndex(0);

            setSelectedItemIndex(miniResultItems.at(index)->getIndexInList());
            miniResultItems.at(index)->setIsSelected(true);
        }
    }
}

int SingleJumpsResultsWindow::getMaxNumberOfWindsForChart() const
{
    return maxNumberOfWindsForChart;
}

void SingleJumpsResultsWindow::setMaxNumberOfWindsForChart(int newMaxNumberOfWindsForChart)
{
    maxNumberOfWindsForChart = newMaxNumberOfWindsForChart;
}

int SingleJumpsResultsWindow::getMaxNumberOfLandingsForChart() const
{
    return maxNumberOfLandingsForChart;
}

int SingleJumpsResultsWindow::getSelectedItemIndex() const
{
    return selectedItemIndex;
}

void SingleJumpsResultsWindow::setSelectedItemIndex(int newSelectedItemIndex)
{
    for(auto & item : miniResultItems)
        item->setIsSelected(false);
    selectedItemIndex = newSelectedItemIndex;
}

QVector<SingleJumpMiniResultWidget *> SingleJumpsResultsWindow::getMiniResultItems() const
{
    return miniResultItems;
}

void SingleJumpsResultsWindow::setMiniResultItems(const QVector<SingleJumpMiniResultWidget *> &newMiniResultItems)
{
    miniResultItems = newMiniResultItems;
}

int SingleJumpsResultsWindow::getMaxNumberOfJudgesForChart() const
{
    return maxNumberOfJudgesForChart;
}

int SingleJumpsResultsWindow::getMaxNumberOfDistancesForChart() const
{
    return maxNumberOfDistancesForChart;
}
