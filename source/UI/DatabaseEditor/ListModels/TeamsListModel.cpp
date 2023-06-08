#include "TeamsListModel.h"

#include "../../../global/CountryFlagsManager.h"
#include <QIcon>
#include <QFont>

TeamsListModel::TeamsListModel(QVector<Team> *teamsVectorPointer, QObject *parent)
    : teamsVectorPointer(teamsVectorPointer), QAbstractListModel(parent)
{
}

QVariant TeamsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int TeamsListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return teamsVectorPointer->size();

    // FIXME: Implement me!
}

QVariant TeamsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole){
        return teamsVectorPointer->at(index.row()).getCountryCode();
    }
    else if(role == Qt::DecorationRole){
        return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(teamsVectorPointer->at(index.row()).getCountryCode().toLower())));
    }
    else if(role == Qt::FontRole){
        return QFont("Quicksand Medium", 11, 600);
    }
    else if(role == Qt::BackgroundRole){
        return QColor(qRgb(253, 253, 253));
    }
    else if(role == Qt::ForegroundRole){
        return QColor(qRgb(30, 30, 30));
    }

    // FIXME: Implement me!
    return QVariant();
}

bool TeamsListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool TeamsListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

QVector<Team> *TeamsListModel::getTeamsVectorPointer() const
{
    return teamsVectorPointer;
}

void TeamsListModel::setTeamsVectorPointer(QVector<Team> *newTeamsVectorPointer)
{
    teamsVectorPointer = newTeamsVectorPointer;
}
