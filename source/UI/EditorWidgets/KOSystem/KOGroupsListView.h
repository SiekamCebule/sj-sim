#ifndef KOGROUPSLISTVIEW_H
#define KOGROUPSLISTVIEW_H

#include <QWidget>
#include <QVector>
#include "../../../competitions/KOSystem/KOGroup.h"
#include "KOGroupInfoWidget.h"

namespace Ui {
class KOGroupsListView;
}

class KOGroupsListView : public QWidget
{
    Q_OBJECT

public:
    explicit KOGroupsListView(QWidget *parent = nullptr);
    ~KOGroupsListView();

    void fillListLayout();

private:
    Ui::KOGroupsListView *ui;

    QVector<KOGroupInfoWidget *> groupWidgets;

    QVector<KOGroup> * KOGroups;
public:
    QVector<KOGroup> *getKOGroups() const;
    void setKOGroups(QVector<KOGroup> *newKOGroups);
};

#endif // KOGROUPSLISTVIEW_H
