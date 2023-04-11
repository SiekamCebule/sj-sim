#include "IndividualStartListEditorModel.h"
#include "../../../simulator/Jumper.h"
#include "../../../global/CountryFlagsManager.h"
#include <QIcon>
#include <QPixmap>
#include <QFont>

IndividualStartListEditorModel::IndividualStartListEditorModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant IndividualStartListEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

bool IndividualStartListEditorModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int IndividualStartListEditorModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return jumpers->size();

    // FIXME: Implement me!
}

QVariant IndividualStartListEditorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role){
    case Qt::DisplayRole:
        return QString::number(index.row() + 1) + ". " + jumpers->at(index.row()).getNameAndSurname();
    case Qt::DecorationRole:
        return QIcon(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumpers->at(index.row()).getCountryCode().toLower())));
    case Qt::ForegroundRole:
        return QColor(qRgb(30, 30, 30));
    case Qt::BackgroundRole:
        return QColor(qRgb(253, 253, 253));
    case Qt::FontRole:
        return QFont("Quicksand Medium", 11, 600);
    }

    return QVariant();
}

bool IndividualStartListEditorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole || role == Qt::DisplayRole){
        if (data(index, role) != value) {
            // FIXME: Implement me!
            emit dataChanged(index, index, {role});
            return true;
        }
    }
    return false;
}

Qt::ItemFlags IndividualStartListEditorModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable; // FIXME: Implement me!
}

bool IndividualStartListEditorModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool IndividualStartListEditorModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

Qt::DropActions IndividualStartListEditorModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QVector<Jumper> *IndividualStartListEditorModel::getJumpers()
{
    return jumpers;
}

void IndividualStartListEditorModel::setJumpers(QVector<Jumper> *newJumpers)
{
    jumpers = newJumpers;
}
