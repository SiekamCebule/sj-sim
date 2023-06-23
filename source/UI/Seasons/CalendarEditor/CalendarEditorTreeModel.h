#ifndef CALENDAREDITORTREEMODEL_H
#define CALENDAREDITORTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include "../../../seasons/SeasonCalendar.h"
#include "../../../global/TreeItem.h"

class CalendarEditorTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CalendarEditorTreeModel(SeasonCalendar * calendar = nullptr, QObject *parent = nullptr);
    ~CalendarEditorTreeModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    SeasonCalendar * calendar;

    TreeItem * rootItem;
public:
    void setupTreeItems();
public:
    SeasonCalendar *getCalendar() const;
    void setCalendar(SeasonCalendar *newCalendar);
    TreeItem *getRootItem() const;
};

#endif // CALENDAREDITORTREEMODEL_H
