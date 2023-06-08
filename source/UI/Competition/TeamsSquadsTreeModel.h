#ifndef TEAMSSQUADSTREEMODEL_H
#define TEAMSSQUADSTREEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include "../../simulator/Jumper.h"
#include "../../simulator/Team.h"
#include "../../global/TreeItem.h"

class TeamsSquadsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TeamsSquadsTreeModel(QVector<Team> * teams, int jumpersInTeam, QObject *parent = nullptr);
    ~TeamsSquadsTreeModel();

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
    int jumpersInTeam;
    QVector<Team> * teams;

    TreeItem * rootItem;
public:
    void deleteAllItems();
    void deleteItem(TreeItem * item);
    void updateItemsByTeamsVector();
    void debugAllItems();
public:
    int getJumpersInTeam() const;
    void setJumpersInTeam(int newJumpersInTeam);
    QVector<Team> *getTeams() const;
    void setTeams(QVector<Team> *newTeams);
    TreeItem *getRootItem() const;
    void setRootItem(TreeItem *newRootItem);
};

#endif // TEAMSSQUADSTREEMODEL_H

/*będzie to działać tak:
 *Sprawdzamy każdy item wywołując funkcję sprawdzającą czy dany item ma jeszcze dzieci. Jeśli tak, to rekursywnie wywołuje kolejny raz. Jeśli nie ma już dzieci to usuwa każde swoje dziecko .
*/
