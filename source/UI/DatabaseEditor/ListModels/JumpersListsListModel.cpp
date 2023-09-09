#include "JumpersListsListModel.h"
#include <QFont>

JumpersListsListModel::JumpersListsListModel(QVector<SaveJumpersList> *jumpersLists, QObject *parent)
    : QAbstractListModel(parent), jumpersLists(jumpersLists)
{
}

QVariant JumpersListsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int JumpersListsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return jumpersLists->count();
}

QVariant JumpersListsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole){
        return jumpersLists->at(index.row()).getName() + " (" + QString::number(jumpersLists->at(index.row()).getJumpers().count()) + tr(" zawodników)");
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

bool JumpersListsListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool JumpersListsListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

QVector<SaveJumpersList> *JumpersListsListModel::getJumpersLists() const
{
    return jumpersLists;
}

void JumpersListsListModel::setJumpersLists(QVector<SaveJumpersList> *newJumpersLists)
{
    jumpersLists = newJumpersLists;
}
