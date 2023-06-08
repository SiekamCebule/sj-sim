#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVector>
#include <QVariant>

class TreeItem
{
public:
    TreeItem(const QVector<QVariant> & dataVector, TreeItem * parentItem = nullptr);
private:
    TreeItem * parentItem;
    QVector<TreeItem*> childItems;
    QVector<QVariant> dataVector;
public:
    void addChild(TreeItem * child);
    QVariant data(int index) const;
    TreeItem * getParentItem();
    TreeItem * getChildItem(int index);
    int row() const;
    int columnCount() const;
    int childCount() const;
    QVector<TreeItem *> getChildItemsReference();
};

#endif // TREEITEM_H