#ifndef SIMULATIONSAVESWINDOW_H
#define SIMULATIONSAVESWINDOW_H

#include <QDialog>
#include "ItemModels/SimulationSavesListModel.h"

namespace Ui {
class SimulationSavesWindow;
}

class SimulationSavesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationSavesWindow(QWidget *parent = nullptr);
    ~SimulationSavesWindow();

private slots:
    void on_listView_simulationSaves_doubleClicked(const QModelIndex &index);

    void on_pushButton_add_clicked();

private:
    Ui::SimulationSavesWindow *ui;
    SimulationSavesListModel * listModel;
public:
};

#endif // SIMULATIONSAVESWINDOW_H