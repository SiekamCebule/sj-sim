#include "CalendarEditorTableModel.h"
#include "../../../global/CountryFlagsManager.h"
#include <QFont>
#include <QIcon>
#include <QPixmap>

CalendarEditorTableModel::CalendarEditorTableModel(SeasonCalendar *calendar, QVector<Hill> *hillsList, QVector<CompetitionRules> *rulesList, QObject *parent)
    : QAbstractTableModel(parent),
      calendar(calendar),
      hillsList(hillsList),
      rulesList(rulesList)
{
}

QVariant CalendarEditorTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch(section){
            case 0:
                return tr("Skocznia");
            case 1:
                return tr("Rodzaj konkursu");
            case 2:
                return tr("Rodzaj serii");
            case 3:
                return tr("Seria próbna");
            case 4:
                return tr("Treningi");
            case 5:
                return tr("Zasady konkursu");
            }
        }
        else if(orientation == Qt::Vertical){
            return QString::number(section + 1);
        }
    }
    return QVariant();
}

int CalendarEditorTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    int count = 0;
    for(auto & comp : calendar->getCompetitionsReference()){
        if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition)
            count++;
    }
    return count;
}

int CalendarEditorTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6;
}

QVariant CalendarEditorTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CompetitionInfo * competition = nullptr;
    int i=0;
    for(auto & comp : calendar->getCompetitionsReference()){ //znajdujemy odpowiedni konkurs z listy (Bo nie pokazujemy treningów i serii próbnych)
        if(comp->getSerieType() == CompetitionInfo::Qualifications || comp->getSerieType() == CompetitionInfo::Competition){
            if(i == index.row()){
                competition = comp;
                break;
            }
            i++;
        }
    }

    if(role == Qt::DisplayRole){
        if(competition != nullptr){
            switch(index.column()){
            case 0:
                return competition->getHill()->getName() + " HS" + QString::number(competition->getHill()->getHSPoint());
            case 1:
                if(competition->getRules().getCompetitionType() == CompetitionRules::Individual)
                    return tr("Indywidualny");
                else if(competition->getRules().getCompetitionType() == CompetitionRules::Team)
                    return tr("Drużynowy");
                break;
            case 2:
                if(competition->getSerieType() == CompetitionInfo::Qualifications)
                    return tr("Kwalifikacje");
                else if(competition->getSerieType() == CompetitionInfo::Competition)
                    return tr("Konkurs");
                break;
            case 3:{
                int index = calendar->getCompetitionsReference().indexOf(competition);
                if(index > 0)
                    if(calendar->getCompetitionsReference()[index - 1]->getSerieType() == CompetitionInfo::TrialRound) //Jeżeli jest seria próbna przed konkursem
                        return tr("TAK") + " - " + calendar->getCompetitionsReference()[index - 1]->getRulesPointer()->getName();
                return tr("NIE");
            }
            case 4:{
                int count = 0; //Liczba treningów przed konkursem
                int indexOfCompetition = calendar->getCompetitionsReference().indexOf(competition);
                if(indexOfCompetition > 0){
                    if(competition->getTrialRound() != nullptr){
                        for(int i=(indexOfCompetition-2); i>=0; i--){ //pomijamy konkurs przez odjęcie '1'
                            if(calendar->getCompetitionsReference()[i]->getSerieType() == CompetitionInfo::Training)
                                count++;
                            else
                                break;
                        }
                    }
                    else{
                        for(int i=(indexOfCompetition-1); i>=0; i--){ //pomijamy konkurs przez odjęcie '1'
                            if(calendar->getCompetitionsReference()[i]->getSerieType() == CompetitionInfo::Training)
                                count++;
                            else
                                break;
                        }
                    }
                }
                if(count == 0)
                    return 0;
                else if(calendar->getCompetitionsReference()[indexOfCompetition]->getTrialRound() != nullptr)
                    return QString::number(count) + " - " + calendar->getCompetitionsReference()[indexOfCompetition - 2]->getRulesPointer()->getName();
                else
                    return QString::number(count) + " - " + calendar->getCompetitionsReference()[indexOfCompetition - 1]->getRulesPointer()->getName();
            }
            case 5:
                return competition->getRules().getName();
            default:
                break;
            }
        }
    }
    else if(role == Qt::DecorationRole){
        if(index.column() == 0){
            if(competition != nullptr){
                return CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(competition->getHill()->getCountryCode().toLower())).scaled(QSize(33, 18));
            }
        }
    }
    else if(role == Qt::FontRole){
        QFont font("Quicksand Medium");
        switch(index.column()){
        case 0:
            font.setPixelSize(14);
            font.setBold(true);
            break;
        case 1:
            font.setPixelSize(12);
            break;
        case 2:
            font.setPixelSize(12);
            break;
        case 3:
            font.setPixelSize(12);
            break;
        case 4:
            font.setPixelSize(12);
            break;
        case 5:
            font.setPixelSize(12);
            font.setItalic(true);
            break;
        }
        return font;
    }
    else if(role == Qt::ForegroundRole){
        if(index.column() == 3){
            if(competition != nullptr){
                int index = calendar->getCompetitionsReference().indexOf(competition);
                if(index > 0)
                    if(calendar->getCompetitionsReference()[index - 1]->getSerieType() == CompetitionInfo::TrialRound) //Jeżeli jest seria próbna przed konkursem
                        return QColor(qRgb(55, 102, 54));
                return QColor(qRgb(120, 50, 52));
            }
        }
        return QColor(qRgb(40, 40, 40));
    }
    else if(role == Qt::BackgroundRole){
        return QColor(qRgb(255, 255, 255));
    }
    else if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }

    return QVariant();
}

bool CalendarEditorTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool CalendarEditorTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool CalendarEditorTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool CalendarEditorTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

QVector<CompetitionRules> *CalendarEditorTableModel::getRulesList() const
{
    return rulesList;
}

void CalendarEditorTableModel::setRulesList(QVector<CompetitionRules> *newRulesList)
{
    rulesList = newRulesList;
}

QVector<Hill> *CalendarEditorTableModel::getHillsList() const
{
    return hillsList;
}

void CalendarEditorTableModel::setHillsList(QVector<Hill> *newHillsList)
{
    hillsList = newHillsList;
}

SeasonCalendar *CalendarEditorTableModel::getCalendar() const
{
    return calendar;
}

void CalendarEditorTableModel::setCalendar(SeasonCalendar *newCalendar)
{
    calendar = newCalendar;
}
