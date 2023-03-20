#ifndef SIMULATIONSETTINGSWINDOW_H
#define SIMULATIONSETTINGSWINDOW_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class SimulationSettingsWindow;
}

class SimulationSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationSettingsWindow(QWidget *parent = nullptr);
    ~SimulationSettingsWindow();

    void fillInputs();
    void resetInputs();

signals:
    void submitted();

private:
    Ui::SimulationSettingsWindow *ui;

public:
    double getTakeoffRatingTakeoffHeightAbsValueFromInput() const;
    double getTakeoffRatingBaseRandomBaseValueFromInput() const;
    double getTakeoffRatingBaseRandomDeviationBaseValueFromInput() const;
    double getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput() const;
    double getTakeoffRatingLogRandomBaseValueFromInput() const;
    double getTakeoffRatingLogRandomDeviationBaseValueFromInput() const;
    double getTakeoffRatingLogRandomDeviationJumpsEqualityDividerFromInput() const;
    double getFlightRatingFlightHeightAbsValueFromInput() const;
    double getFlightRatingBaseRandomBaseValueFromInput() const;
    double getFlightRatingBaseRandomDeviationBaseValueFromInput() const;
    double getFlightRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput() const;
    double getFlightRatingLogRandomBaseValueFromInput() const;
    double getFlightRatingLogRandomDeviationBaseValueFromInput() const;
    double getFlightRatingLogRandomDeviationJumpsEqualityDividerFromInput() const;
private slots:
    void on_pushButton_submit_2_clicked();
};

#endif // SIMULATIONSETTINGSWINDOW_H
