#ifndef GENERALCLASSIFICATIONTABLEMODEL_H
#define GENERALCLASSIFICATIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QPair>
#include "../../../../simulator/Jumper.h"

class GeneralClassificationTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GeneralClassificationTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<QPair<Jumper *, double>> results;
public:
    QVector<QPair<Jumper *, double> > getResults() const;
    void setResults(const QVector<QPair<Jumper *, double> > &newResults);
};

#endif // GENERALCLASSIFICATIONTABLEMODEL_H
