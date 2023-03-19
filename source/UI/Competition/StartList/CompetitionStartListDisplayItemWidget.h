#ifndef COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H
#define COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H

#include <QWidget>
#include "../../../simulator/Jumper.h"
#include "../../../simulator/Team.h"

namespace Ui {
class CompetitionStartListDisplayItemWidget;
}

class CompetitionStartListDisplayItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionStartListDisplayItemWidget(short competitiorType, QWidget *parent = nullptr);
    ~CompetitionStartListDisplayItemWidget();

    void setIsActive(bool arg);
    bool getIsActive();
    void fillWidget();

signals:
    void selectionChanged();
    void selected();
    void doubleClicked();
    void activityChanged();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_checkBox_active_stateChanged(int arg1);

private:
    Ui::CompetitionStartListDisplayItemWidget *ui;

    Jumper * jumper;
    Team * team;
    int indexInParentList;
    bool isSelected;

    short competitiorType;

public:
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    Team *getTeam() const;
    void setTeam(Team *newTeam);
    int getIndexInParentList() const;
    void setIndexInParentList(int newIndexInParentList);
    bool getIsSelected() const;
    void setIsSelected(bool newIsSelected);
    short getCompetitiorType() const;
    void setCompetitiorType(short newCompetitiorType);
};

#endif // COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H
