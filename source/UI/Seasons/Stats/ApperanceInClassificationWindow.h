#ifndef APPERANCEINCLASSIFICATIONWINDOW_H
#define APPERANCEINCLASSIFICATIONWINDOW_H

#include <QDialog>
#include <QString>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include "../../../seasons/Season.h"
class Jumper;
class Classification;
class CompetitionInfo;

namespace Ui {
class ApperanceInClassificationWindow;
}

class ApperanceInClassificationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ApperanceInClassificationWindow(QWidget *parent = nullptr);
    ~ApperanceInClassificationWindow();

    void fillWindow();

    void setupChart();
    void fillChart();

private:
    Ui::ApperanceInClassificationWindow *ui;

    Season * season;
    Classification * classification;
    Jumper * jumper;
    QString teamCode;

    QChart * chart;
    QChartView * chartView;
    QLineSeries * series;

    QHash<CompetitionInfo *, int> archiveResults;
public:
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    QString getTeamCode() const;
    void setTeamCode(const QString &newTeamCode);
    Classification *getClassification() const;
    void setClassification(Classification *newClassification);
    QHash<CompetitionInfo *, int> getArchiveResults() const;
    void setArchiveResults(const QHash<CompetitionInfo *, int> &newArchiveResults);
    Season *getSeason() const;
    void setSeason(Season *newSeason);
};

#endif // APPERANCEINCLASSIFICATIONWINDOW_H
