#include "CompetitionsArchiveListModel.h"
#include "../../../global/CountryFlagsManager.h"
#include <QIcon>
#include <QFont>

CompetitionsArchiveListModel::CompetitionsArchiveListModel(QVector<CompetitionInfo *> *seasonCompetitions, QObject *parent)
    : QAbstractListModel(parent), seasonCompetitions(seasonCompetitions)
{
}

QVariant CompetitionsArchiveListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int CompetitionsArchiveListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    int count = 0;
    for(auto & competition : *seasonCompetitions)
    {
        if(competition->getPlayed() == true)
            count++;
        else
            break;
    }
    return count;
}

QVariant CompetitionsArchiveListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        CompetitionInfo * competition = seasonCompetitions->at(index.row());
        if(competition->getPlayed() == true){
            QString string = QString::number(index.row() + 1) + ". " + competition->getHill()->getName() + " HS" + QString::number(competition->getHill()->getHSPoint()) + " (";
            switch(competition->getSerieType())
            {
            case CompetitionInfo::Competition:
                string += tr("Konkurs");
                break;
            case CompetitionInfo::Qualifications:
                string += tr("Kwalifikacje");
                break;
            case CompetitionInfo::TrialRound:
                string += tr("Seria próbna");
                    break;
            case CompetitionInfo::Training:
                string += tr("Trening");
                break;
            }
            string += ")";
            return string;
        }
    }
    else if(role == Qt::DecorationRole)
    {
        CompetitionInfo * competition = seasonCompetitions->at(index.row());
        return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(competition->getHill()->getCountryCode().toLower())));
    }
    else if(role == Qt::FontRole)
    {
        return QFont("Ubuntu", 10);
    }
    else if(role == Qt::ForegroundRole)
        return QColor("black");
    else if(role == Qt::TextAlignmentRole)
        return Qt::AlignLeft;

    return QVariant();
}

bool CompetitionsArchiveListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool CompetitionsArchiveListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

QVector<CompetitionInfo *> *CompetitionsArchiveListModel::getSeasonCompetitions() const
{
    return seasonCompetitions;
}

void CompetitionsArchiveListModel::setSeasonCompetitions(QVector<CompetitionInfo *> *newSeasonCompetitions)
{
    seasonCompetitions = newSeasonCompetitions;
}
