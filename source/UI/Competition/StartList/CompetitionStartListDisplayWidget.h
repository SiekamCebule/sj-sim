#ifndef COMPETITIONSTARTLISTDISPLAYWIDGET_H
#define COMPETITIONSTARTLISTDISPLAYWIDGET_H

#include <QWidget>
#include <QVector>
#include <QSettings>
#include "CompetitionStartListDisplayItemWidget.h"
#include "../../../simulator/Jumper.h"
#include "../../../simulator/Team.h"
namespace Ui {
class CompetitionStartListDisplayWidget;
}

class CompetitionStartListDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionStartListDisplayWidget(QWidget *parent = nullptr);
    ~CompetitionStartListDisplayWidget();

    enum CompetitiorsType{
        Jumpers,
        Teams,
        Null
    };
    void fillItemsLayout();
    void fillCompetitiorsActivity();
    void setupTeamsJumpersVectors();

    static QVector<Jumper*> convertToVectorObjectOfPointers(QVector<Jumper> *jumpers);

signals:
    void competitiorsTypeChanged();
public slots:
    void askForNewIndex();

private slots:
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_edit_clicked();

private:
    Ui::CompetitionStartListDisplayWidget *ui;

    QVector<CompetitionStartListDisplayItemWidget *> items;
    QVector<Jumper> * jumpers;
    QVector<Team> * teams;

    QVector<bool> jumpersActivity;
    QVector<bool> teamsActivity;
    QVector<bool> * competitiorsActivity;


    int selectedItem;
    short competitiorsType;
    void updateItemsSelection();

    QSettings settings;

public:
    void setJumpers(QVector<Jumper> *newJumpers);
    QVector<Jumper> *getJumpers() const;
    void setTeams(QVector<Team> *newTeams);
    short getCompetitiorsType() const;
    void setCompetitiorsType(short newCompetitiorsType);
};

#endif // COMPETITIONSTARTLISTDISPLAYWIDGET_H
