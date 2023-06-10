#include "TeamResultsTreeModel.h"
#include <QFont>
#include <QPixmap>
#include "../../../global/CountryFlagsManager.h"

TeamResultsTreeModel::TeamResultsTreeModel(TeamCompetitionManager * manager, QObject *parent)
    : QAbstractItemModel(parent), manager(manager), results(manager->getResults()), teams(&manager->getRoundsTeamsReference()[0]), teamsAdvanceStatuses(&manager->getTeamsAdvanceStatusesReference())
{
    rootItem = new TreeItem({tr("Miejsce"), tr("Drużyna"), tr("Zawodnik"), tr("Punkty")});
}

QVariant TeamResultsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex TeamResultsTreeModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex TeamResultsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->getParentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TeamResultsTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    if(parentItem != rootItem)
        return results->howManyJumpersJumpedInTeam(teams->at(parent.row())); //ilość zawodników którzy NA RAZIE skoczyli w danej drużynie.
    return parentItem->childCount(); //naglowki druzyn (parent == rootItem)
}

int TeamResultsTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant TeamResultsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->data(index.column());
    }
    else if(role == Qt::DecorationRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if(index.column() == 1 && item->getParentItem() == rootItem){
            int teamIndex = 0;
            if(item->getParentItem() == rootItem)
                teamIndex = item->row();
            else teamIndex = item->getParentItem()->row();
            QPixmap flagPixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(results->getResultsReference()[index.row()].getTeam()->getCountryCode().toLower())).scaled(QSize(24, 14));
            if(item->getParentItem() == rootItem)
            {
                flagPixmap = flagPixmap.scaled(QSize(35, 21));
            }
            return flagPixmap;
        }
    }
    else if(role == Qt::FontRole){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        QFont font("Quicksand Medium");
        font.setPointSize(8);
        if(item->getParentItem() == rootItem){ //jeśli jest to nagłówek drużyny
            font.setPointSize(11);
            if(index.column() == 3)
                font.setBold(true);
        }
        return font;
    }
    else if(role == Qt::ForegroundRole){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if(item->getParentItem() == rootItem)
            return QColor(qRgb(0, 0, 0));
        else
            return QColor(qRgb(30, 30, 30));
    }
    else if(role == Qt::BackgroundRole){
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        Team * team = results->getResultsReference()[index.row()].getTeam();
        if(item->getParentItem() == rootItem){
            if(manager->getAdvanceStatusOfTeam(team) == StartListCompetitorStatus::SureDroppedOut){
                return QColor(qRgb(252, 237, 237));
            }
            else if(results->getResultByIndex(index.row())->getJumpsReference().count() < manager->getActualRound() * manager->getActualGroup()) //Ilość skoków jest mniejsza niż round * group
            {
                return QColor(qRgb(250, 250, 250));
            }
            else if(manager->getAdvanceStatusOfTeam(team) == StartListCompetitorStatus::Waiting){
                return QColor(qRgb(253, 253, 249));
            }
            else if(manager->getAdvanceStatusOfTeam(team) == StartListCompetitorStatus::SureAdvanced){
                return QColor(qRgb(242, 255, 246));
            }
        }
    }

    return QVariant();
}

TreeItem *TeamResultsTreeModel::getRootItem() const
{
    return rootItem;
}

void TeamResultsTreeModel::setupTreeItems()
{
    TreeItem::deleteAllTreeItemsRecursively(rootItem);
    rootItem = new TreeItem({tr("Miejsce"), tr("Drużyna"), tr("Zawodnik"), tr("Punkty")});
    for(auto & res : results->getResultsReference()){
        TreeItem * teamHeaderItem = new TreeItem({QString::number(res.getPosition()), res.getTeam()->getCountryCode(), "", QString::number(res.getPointsSum())}, rootItem);
        for(auto & jumper : res.getTeam()->getJumpersReference()){
            //Czy w resultsach jest już ten zawodnik?
            bool resultsContainsJumper = false;
            for(auto & jump : res.getJumps())
                if(jump.getJumper() == jumper){
                    resultsContainsJumper = true;
                    break;
                }
            if(resultsContainsJumper == true){
                double jumperPointsSum = 0;
                for(auto & jump : res.getJumps()){
                    if(jump.getJumper() == jumper)
                        jumperPointsSum += jump.getPoints();
                }
                TreeItem * jumperItem = new TreeItem({"", "", jumper->getNameAndSurname(), QString::number(jumperPointsSum)}, teamHeaderItem);
            }
        }
    }
}

TeamCompetitionManager *TeamResultsTreeModel::getManager() const
{
    return manager;
}

void TeamResultsTreeModel::setManager(TeamCompetitionManager *newManager)
{
    manager = newManager;
}

QVector<QPair<Team *, int> > *TeamResultsTreeModel::getTeamsAdvanceStatuses() const
{
    return teamsAdvanceStatuses;
}

void TeamResultsTreeModel::setTeamsAdvanceStatuses(QVector<QPair<Team *, int> > *newTeamsAdvanceStatuses)
{
    teamsAdvanceStatuses = newTeamsAdvanceStatuses;
}
