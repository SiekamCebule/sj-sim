#ifndef COMPETITIONSRANGECOMBOBOXESWIDGET_H
#define COMPETITIONSRANGECOMBOBOXESWIDGET_H

#include <QWidget>
#include <QVector>
#include "../../../../competitions/CompetitionInfo.h"
#include "../../../../seasons/Season.h"

namespace Ui {
class CompetitionsRangeComboBoxesWidget;
}

class CompetitionsRangeComboBoxesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionsRangeComboBoxesWidget(QWidget *parent = nullptr);
    ~CompetitionsRangeComboBoxesWidget();

    void setupComboBoxes();
    CompetitionInfo * getCompetition(int which);

    QVector<Season> *getSeasonsList() const;
    void setSeasonsList(QVector<Season> *newSeasonsList);

signals:
    void changed();

private:
    Ui::CompetitionsRangeComboBoxesWidget *ui;
    QVector<Season> * seasonsList;
};

#endif // COMPETITIONSRANGECOMBOBOXESWIDGET_H
