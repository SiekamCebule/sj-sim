#ifndef COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H
#define COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H

#include <QWidget>
#include "../../../simulator/Jumper.h"

namespace Ui {
class CompetitionStartListDisplayItemWidget;
}

class CompetitionStartListDisplayItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionStartListDisplayItemWidget(QWidget *parent = nullptr);
    ~CompetitionStartListDisplayItemWidget();

    void setIsActive(bool arg);
    bool getIsActive();
    void fillWidget();

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);

    int getIndexInParentList() const;
    void setIndexInParentList(int newIndexInParentList);

    void uninstallEventFilterForThis();

    bool getIsSelected() const;
    void setIsSelected(bool newIsSelected);

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
    int indexInParentList;
    bool isSelected;
};

#endif // COMPETITIONSTARTLISTDISPLAYITEMWIDGET_H
