#ifndef COMPETITIONSTARTLISTDISPLAYWIDGET_H
#define COMPETITIONSTARTLISTDISPLAYWIDGET_H

#include <QWidget>
#include <QVector>
#include "CompetitionStartListDisplayItemWidget.h"
#include "../../../simulator/Jumper.h"
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
        Teams
    };

    void fillItemsLayout();
    void setJumpers(QVector<Jumper> *newJumpers);
    void fillCompetitiorsActivity();

    short getCompetitiorsType() const;
    void setCompetitiorsType(short newCompetitiorsType);

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
    QVector<bool> competitiorsActivity;

    int selectedItem;
    short competitiorsType;
    void updateItemsSelection();
};

#endif // COMPETITIONSTARTLISTDISPLAYWIDGET_H
