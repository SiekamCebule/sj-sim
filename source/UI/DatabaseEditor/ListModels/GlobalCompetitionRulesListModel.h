#ifndef GLOBALCOMPETITIONRULESLISTMODEL_H
#define GLOBALCOMPETITIONRULESLISTMODEL_H

#include <QAbstractListModel>
class CompetitionRules;

class GlobalCompetitionRulesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GlobalCompetitionRulesListModel(QVector<CompetitionRules> * globalCompetitonRulesVectorPointer, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QVector<CompetitionRules> * globalCompetitonRulesVectorPointer;
public:
    QVector<CompetitionRules> *getGlobalCompetitonRulesVectorPointer() const;
    void setGlobalCompetitonRulesVectorPointer(QVector<CompetitionRules> *newGlobalCompetitonRulesVectorPointer);
};

#endif // GLOBALCOMPETITIONRULESLISTMODEL_H
