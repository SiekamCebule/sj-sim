#include "CompetitionsRangeComboBoxesWidget.h"
#include "ui_CompetitionsRangeComboBoxesWidget.h"
#include <QIcon>
#include "../../../../global/CountryFlagsManager.h"

CompetitionsRangeComboBoxesWidget::CompetitionsRangeComboBoxesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionsRangeComboBoxesWidget)
{
    ui->setupUi(this);

    connect(ui->comboBox_firstCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
    connect(ui->comboBox_secondCompetition, &QComboBox::currentIndexChanged, this, &CompetitionsRangeComboBoxesWidget::changed);
}

CompetitionsRangeComboBoxesWidget::~CompetitionsRangeComboBoxesWidget()
{
    delete ui;
}

void CompetitionsRangeComboBoxesWidget::setupComboBoxes()
{
    ui->comboBox_firstCompetition->clear();
    ui->comboBox_secondCompetition->clear();
    int seasonIndex=1;
    for(auto & season : *seasonsList)
    {
        int competitionIndex = 1;
        QString string = "--- Sezon " + QString::number(season.getSeasonNumber()) + " ---";
        ui->comboBox_firstCompetition->addItem(string);
        ui->comboBox_secondCompetition->addItem(string);
        for(auto & competition : season.getCalendarReference().getCompetitionsReference())
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
        seasonIndex++;
    }
    if(ui->comboBox_firstCompetition->count() > 0)
        ui->comboBox_firstCompetition->setCurrentIndex(1);
    ui->comboBox_secondCompetition->setCurrentIndex(ui->comboBox_secondCompetition->count() - 1);
}

CompetitionInfo *CompetitionsRangeComboBoxesWidget::getCompetition(int which)
{
    int index = 0;
    switch(which)
    {
    case 1:
        index = ui->comboBox_firstCompetition->currentIndex() - 1;
        break;
    case 2:
        index = ui->comboBox_secondCompetition->currentIndex() - 1;
        break;
    }
    if(index == -1)
        index = 0;
    if(index >= 0){
        //Teraz odejmijmy index za każdy sezon
        for(auto & season : *seasonsList)
        {
            if(index >= season.getCalendarReference().getCompetitionsReference().count())
            {
                index -= season.getCalendarReference().getCompetitionsReference().count();
                continue;
            }
            else
            {
                qDebug()<<season.getCalendarReference().getCompetitionsReference()[index]->getHill()->getName();
                return season.getCalendarReference().getCompetitionsReference()[index];
            }
        }
    }
    return nullptr;

    //sezon1 ma 68
    //index 67
}

QVector<Season> *CompetitionsRangeComboBoxesWidget::getSeasonsList() const
{
    return seasonsList;
}

void CompetitionsRangeComboBoxesWidget::setSeasonsList(QVector<Season> *newSeasonsList)
{
    seasonsList = newSeasonsList;
}
