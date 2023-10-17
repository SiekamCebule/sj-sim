#include "CompetitionsRangeComboBoxesWidget.h"
#include "ui_CompetitionsRangeComboBoxesWidget.h"
#include <QIcon>
#include "../../../../global/CountryFlagsManager.h"

CompetitionsRangeComboBoxesWidget::CompetitionsRangeComboBoxesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionsRangeComboBoxesWidget)
{
    ui->setupUi(this);
}

CompetitionsRangeComboBoxesWidget::~CompetitionsRangeComboBoxesWidget()
{
    delete ui;
}

void CompetitionsRangeComboBoxesWidget::setupConnections()
{
    connect(ui->comboBox_firstCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
    connect(ui->comboBox_secondCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
}

void CompetitionsRangeComboBoxesWidget::setupComboBoxes()
{
    qDebug()<<QString("setup............. (%1, %2)").arg(calendarFilter).arg(mergeCalendars);
    disconnect(ui->comboBox_firstCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
    disconnect(ui->comboBox_secondCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
    ui->comboBox_firstCompetition->clear();
    ui->comboBox_secondCompetition->clear();
    int seasonIndex=1;
    for(auto & season : *seasonsList)
    {
        int competitionIndex = 1;
        QString string = "--- Sezon " + QString::number(season.getSeasonNumber()) + " ---";
        ui->comboBox_firstCompetition->addItem(string);
        ui->comboBox_secondCompetition->addItem(string);
        for(auto & cal : season.getCalendarsReference())
        {
            if(cal->getName() == calendarFilter || calendarFilter == "" || mergeCalendars == true)
            {
                for(auto & competition : cal->getCompetitionsReference())
                {
                    if(competition->getPlayed() == false)
                        break;
                    QString string = "S" + QString::number(seasonIndex) + "C" + QString::number(competitionIndex);
                    string += " - " + competition->getHill()->getName() + " HS" + QString::number(competition->getHill()->getHSPoint());
                    if(competition->getRulesPointer()->getCompetitionType() == CompetitionRules::Team)
                        string += tr(" (D)");
                    switch(competition->getSerieType())
                    {
                    case CompetitionInfo::Competition:
                        string += tr(" (Konk.)");
                        break;
                    case CompetitionInfo::Qualifications:
                        string += tr(" (Kwal.)");
                        break;
                    case CompetitionInfo::TrialRound:
                        string += tr(" (Prób.)");
                            break;
                    case CompetitionInfo::Training:
                        string += tr(" (Tren.)");
                        break;
                    }

                    QPixmap pixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(competition->getHill()->getCountryCode().toLower()));
                    ui->comboBox_firstCompetition->addItem(QIcon(pixmap), string);
                    ui->comboBox_secondCompetition->addItem(QIcon(pixmap), string);
                    competitionIndex++;
                }
                qDebug()<<"kal "<<cal->getName()<<", "<<ui->comboBox_firstCompetition->count();
                qDebug()<<"count: "<<ui->comboBox_firstCompetition->count();
            }
        }
        seasonIndex++;
    }
    if(ui->comboBox_firstCompetition->count() > 0)
        ui->comboBox_firstCompetition->setCurrentIndex(1);
    ui->comboBox_secondCompetition->setCurrentIndex(ui->comboBox_secondCompetition->count() - 1);
    setupConnections();
}

bool CompetitionsRangeComboBoxesWidget::getMergeCalendars() const
{
    return mergeCalendars;
}

void CompetitionsRangeComboBoxesWidget::setMergeCalendars(bool newMergeCalendars)
{
    mergeCalendars = newMergeCalendars;
}

/*
 * Sezon 2023 (0)
 * K1 (1)
 * K2 (2)
 * Sezon 2024 (3)
 * K1 (4)
 * K2 (5)
 * Sezon 2025 (6)
 * K1 (7)
 * K2 (8)
*/
CompetitionInfo *CompetitionsRangeComboBoxesWidget::getCompetition(int which)
{
    qDebug()<<"GET COMPETITION NUMBER "<<which;
    qDebug()<<"filter: "<<calendarFilter;
    int index = 0;
    switch(which)
    {
    case 1:
        index = ui->comboBox_firstCompetition->currentIndex();
        break;
    case 2:
        index = ui->comboBox_secondCompetition->currentIndex();
        break;
    }
    if(index == -1)
        index = 0;
    int i=0;
    if(index >= 0){
        //Teraz odejmijmy index za każdy sezon
        for(auto & season : *seasonsList)
        {
            qDebug()<<"na początku sezonu: "<<index;
            qDebug()<<"season: "<<season.getSeasonNumber();
            if(((season.containsCalendarByName(calendarFilter) == false && calendarFilter != "") && mergeCalendars == false) && index > 0){
                qDebug()<<QString("not contains calendar by name (%1)").arg(calendarFilter);
                index -= 1;
                continue;
            }
            qDebug()<<"po odjeciu za brak bycia w sezonie: "<<index;
            //else index -= 1;
            for(auto & cal : season.getCalendarsReference())
            {
                qDebug()<<"cal: "<<cal->getName();
                if(cal->getName() == calendarFilter || calendarFilter == "" || mergeCalendars == true)
                {
                    qDebug()<<"jest taka jak filter lub brak filtra:";
                    if(index > cal->getCompetitionsReference().count() && cal->getCompetitionsReference().count() > 0)
                    {
                        qDebug()<<"konieczność odjęcia";
                        index -= cal->howManyCompetitionsPlayed();
                        //index -= 1;
                        qDebug()<<"po odjęciu: "<<index<<QString("(-%1)").arg(cal->getCompetitionsReference().count());
                        i++;
                    }
                    else
                    {
                        qDebug()<<"Zaraz return";
                        if(index > 0)
                            index--;
                        qDebug()<<"return index: "<<index;
                        return cal->getCompetitionsReference()[index];
                    }
                }
            }
            index--;
        }
    }
    return nullptr;
}

QVector<Season> *CompetitionsRangeComboBoxesWidget::getSeasonsList() const
{
    return seasonsList;
}

void CompetitionsRangeComboBoxesWidget::setSeasonsList(QVector<Season> *newSeasonsList)
{
    seasonsList = newSeasonsList;
}

QString CompetitionsRangeComboBoxesWidget::getCalendarFilter() const
{
    return calendarFilter;
}

void CompetitionsRangeComboBoxesWidget::setCalendarFilter(const QString &newCalendarFilter)
{
    calendarFilter = newCalendarFilter;
}
