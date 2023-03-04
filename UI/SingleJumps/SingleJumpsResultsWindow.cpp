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
#include <QEventLoop>
#include <QMap>
#include <QDebug>

extern IDGenerator globalIDGenerator;

SingleJumpsResultsWindow::SingleJumpsResultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsResultsWindow),
    maxNumberOfDistancesForChart(0)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

SingleJumpsResultsWindow::~SingleJumpsResultsWindow()
{
    delete ui;
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
        /*qDebug()<<"SingleJumpMiniResultWidget.jumpData: "<<widget->getJumpData();
        qDebug()<<"SingleJumpMiniResultWidget.jumpData.HILL: "<<widget->getJumpData()->getHill();
        qDebug()<<"SingleJumpMiniResultWidget.jumpData.JUMPER: "<<widget->getJumpData()->getJumper();
        qDebug()<<"SingleJumpMiniResultWidget.jumpData.SIMULATOR: "<<widget->getJumpData()->getSimulator();
        qDebug()<<"Widget.adress: "<<widget;
        qDebug()<<"Manager.adress: "<<manager;*/

        ui->verticalLayout_singleJumpResultWidgets->addWidget(widget);
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
            /*qDebug()<<"(connect) SingleJumpMiniResultWidget.jumpData: "<<widget->getJumpData();
            qDebug()<<"(connect) SingleJumpMiniResultWidget.jumpData.JUMPER: "<<widget->getJumpData()->getJumper();
            qDebug()<<"(connect) Widget.adress: "<<widget;
            qDebug()<<"(connect) Manager.adress: "<<manager;
            qDebug()<<"(connect) Manager.jumper.nameAndSurname: "<<manager->getJumper().getNameAndSurname();
            qDebug()<<"(connect) actualJump.points: "<<jump.getPoints();
            qDebug()<<"(connect) SingleJumpMiniResultWidget.jumpData.HILL: "<<widget->getJumpData()->getHill();
            qDebug()<<"(connect) SingleJumpMiniResultWidget.jumpData.DISTANCE: "<<widget->getJumpData()->getDistance();
            qDebug()<<"(connect) SingleJumpMiniResultWidget.jumpData.HILL: "<<widget->getJumpData()->getHill();*/

            jumpInfo->fillJumpInformations();
            ui->verticalLayout_jumpInfos->addWidget(jumpInfo);
        });

        i++;
    }
    ui->verticalLayout_singleJumpResultWidgets->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void SingleJumpsResultsWindow::fillDistancesChart()
{
    QChart * distancesChart = new QChart();
    distancesChart->legend()->hide();
    distancesChart->addSeries(getSplineSeriesForDistancesChart());
    distancesChart->setTitle("Rozkład odległości zawodnika");
    distancesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    distancesChart->createDefaultAxes();
    distancesChart->axes(Qt::Vertical).first()->setRange(1, getMaxNumberOfDistancesForChart() * 1.15);

    QChartView * distancesChartView = new QChartView(distancesChart);
    distancesChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_distanceStatistics->addWidget(distancesChartView);
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

    //return series;
    return series;
}

int SingleJumpsResultsWindow::getMaxNumberOfDistancesForChart() const
{
    return maxNumberOfDistancesForChart;
}
