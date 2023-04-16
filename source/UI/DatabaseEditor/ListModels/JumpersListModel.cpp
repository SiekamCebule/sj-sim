#include "JumpersListModel.h"

#include "../../../simulator/Jumper.h"
#include "../../../global/CountryFlagsManager.h"
#include <QIcon>
#include <QFont>
#include <QColor>

JumpersListModel::JumpersListModel(QVector<Jumper> *jumpersVectorPointer, QObject *parent)
    : QAbstractListModel(parent),
      jumpersVectorPointer(jumpersVectorPointer)
{

}

QVariant JumpersListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

bool JumpersListModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int JumpersListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return jumpersVectorPointer->count();
}

QVariant JumpersListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole){
        return jumpersVectorPointer->at(index.row()).getNameAndSurname();
    }
    else if(role == Qt::DecorationRole){
        return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumpersVectorPointer->at(index.row()).getCountryCode().toLower())));
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

bool JumpersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole){
        if (data(index, role) != value) {
            // FIXME: Implement me!
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

Qt::ItemFlags JumpersListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index); // FIXME: Implement me!
}

bool JumpersListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool JumpersListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

QVector<Jumper> *JumpersListModel::getJumpersVectorPointer()
{
    return jumpersVectorPointer;
}

void JumpersListModel::setJumpersVectorPointer(QVector<Jumper> *newJumpersVectorPointer)
{
    jumpersVectorPointer = newJumpersVectorPointer;
}
