#ifndef RESULTSTABLEMODEL_H
#define RESULTSTABLEMODEL_H

#include <QAbstractTableModel>
#include "../../../competitions/AbstractCompetitionResults.h"
#include "../../../competitions/AbstractCompetitionManager.h"
#include "../../../competitions/CompetitionInfo.h"

class ResultsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ResultsTableModel(int type = 0, AbstractCompetitionResults * results = nullptr, AbstractCompetitionManager * manager = nullptr, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    int type; // 0 - Individual,  1 - Team
    AbstractCompetitionResults * results;
    AbstractCompetitionManager * manager;

public:
    AbstractCompetitionResults *getResults() const;
    void setResults(AbstractCompetitionResults *newResults);
    int getType() const;
    void setType(int newType);
    AbstractCompetitionManager *getManager() const;
    void setManager(AbstractCompetitionManager *newManager);
};

#endif // RESULTSTABLEMODEL_H
