#include "CalendarEditorTreeModel.h"
#include "../../../global/CountryFlagsManager.h"

#include <QFont>

CalendarEditorTreeModel::CalendarEditorTreeModel(SeasonCalendar *calendar, QObject *parent)
    : QAbstractItemModel(parent),
      calendar(calendar)
{
    rootItem = new TreeItem({tr("Skocznia"), tr("Rodzaj")});
}

CalendarEditorTreeModel::~CalendarEditorTreeModel()
{
    TreeItem::deleteAllTreeItemsRecursively(rootItem);
}

QVariant CalendarEditorTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            return rootItem->data(section);
        }
    }
    else if(role == Qt::FontRole){
        QFont font("Quicksand Medium", 11);
        return font;
    }
    else if(role == Qt::ForegroundRole){
        return QColor(qRgb(0, 0, 0));
    }
    return QVariant();
}

QModelIndex CalendarEditorTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->getChildItem(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex CalendarEditorTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->getParentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int CalendarEditorTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int CalendarEditorTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant CalendarEditorTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    else if(role == Qt::DecorationRole){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if(index.column() == 0){

            int competitionInfoIndex = 0;
            int howMany = 0;
            void * wanted = nullptr;

            int parentRow = 0;
            int row = 0;
            while(wanted != index.internalPointer()){
                if(item->getParentItem() == rootItem)
                    wanted = rootItem->getChildItem(parentRow);
                else
                    wanted = rootItem->getChildItem(parentRow)->getChildItem(row);
                row++;
                if(row == rootItem->getChildItem(parentRow)->childCount()){
                    parentRow++;
                    row = 0;
                }
                howMany ++;
            }
            competitionInfoIndex = howMany - 1;
            QPixmap flagPixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(calendar->getCompetitionsReference()[competitionInfoIndex].getHill()->getCountryCode().toLower())).scaled(QSize(24, 14));
            if(item->getParentItem() == rootItem)
            {
                flagPixmap = flagPixmap.scaled(QSize(35, 21));
            }
            return flagPixmap;
        }
    }

    return QVariant();
}

TreeItem *CalendarEditorTreeModel::getRootItem() const
{
    return rootItem;
}

void CalendarEditorTreeModel::setupTreeItems()
{
    TreeItem::deleteAllTreeItemsRecursively(rootItem);
    rootItem = new TreeItem({tr("Skocznia"), tr("Rodzaj")});
    for(auto & weekend : calendar->getCompetitionWeekendsVector()){
        Hill * hill = weekend.first;
        TreeItem * weekendHeaderItem = new TreeItem({hill->getName() + " HS" + QString::number(hill->getHSPoint()), ""}, rootItem);
        for(CompetitionInfo * & competition : weekend.second){
            TreeItem * competitionItem = new TreeItem({hill->getName() + " HS" + QString::number(hill->getHSPoint())});
            if(competition->getRulesPointer()->getCompetitionType() == CompetitionRules::Individual)
                competitionItem->getDataVectorReference().push_back(tr("Indywidualny"));
            else
                competitionItem->getDataVectorReference().push_back(tr("Drużynowy"));
        }
    }
}

SeasonCalendar *CalendarEditorTreeModel::getCalendar() const
{
    return calendar;
}

void CalendarEditorTreeModel::setCalendar(SeasonCalendar *newCalendar)
{
    calendar = newCalendar;
}
