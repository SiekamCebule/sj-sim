#ifndef TEAMSLISTMODEL_H
#define TEAMSLISTMODEL_H

#include <QAbstractListModel>
#include "../../../simulator/Team.h"

class TeamsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TeamsListModel(QVector<Team> * teamsVectorPointer, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QVector<Team> * teamsVectorPointer;
public:
    QVector<Team> *getTeamsVectorPointer() const;
    void setTeamsVectorPointer(QVector<Team> *newTeamsVectorPointer);
};

#endif // TEAMSLISTMODEL_H
