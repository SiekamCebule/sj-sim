#include "TreeItem.h"

TreeItem::TreeItem(const QVector<QVariant> &dataVector, TreeItem *parentItem) :
    parentItem(parentItem), dataVector(dataVector)
{
    if(this->parentItem)
        this->parentItem->addChild(this);
}

void TreeItem::addChild(TreeItem *child)
{
    childItems.append(child);
}

QVariant TreeItem::data(int index) const
{
    if(index < dataVector.size())
        return dataVector[index];
    return QVariant();
}

TreeItem *TreeItem::getParentItem()
{
    return parentItem;
}

TreeItem *TreeItem::getChildItem(int index)
{
    return childItems[index];
}

int TreeItem::row() const
{
    if(parentItem){
        return parentItem->childItems.indexOf(const_cast<TreeItem *>(this));
    }
    return 0;
}

int TreeItem::columnCount() const
{
    return dataVector.count();
}

int TreeItem::childCount() const
{
    return childItems.count();
}

QVector<TreeItem *> TreeItem::getChildItemsReference()
{
    return childItems;
}
