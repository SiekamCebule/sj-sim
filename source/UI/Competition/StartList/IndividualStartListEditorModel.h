#ifndef INDIVIDUALSTARTLISTEDITORMODEL_H
#define INDIVIDUALSTARTLISTEDITORMODEL_H

#include <QAbstractListModel>
#include <QVector>
class Jumper;

class IndividualStartListEditorModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit IndividualStartListEditorModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual Qt::DropActions supportedDropActions() const override;

private:
    QVector<Jumper> * jumpers;
public:
    QVector<Jumper> *getJumpers();
    void setJumpers(QVector<Jumper> *newJumpers);
};

#endif // INDIVIDUALSTARTLISTEDITORMODEL_H
