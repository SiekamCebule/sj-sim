#ifndef NEWSIMULATIONSAVECONFIGURATIONWINDOW_H
#define NEWSIMULATIONSAVECONFIGURATIONWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class NewSimulationSaveConfigurationWindow;
}

class NewSimulationSaveConfigurationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewSimulationSaveConfigurationWindow(QWidget *parent = nullptr);
    ~NewSimulationSaveConfigurationWindow();

    QString getNameFromInput() const;
    int getSeasonNumberFromInput() const;

private slots:
    void on_pushButton_submit_clicked();

private:
    Ui::NewSimulationSaveConfigurationWindow *ui;
};

#endif // NEWSIMULATIONSAVECONFIGURATIONWINDOW_H
