#ifndef SINGLEJUMPSCONFIGWINDOW_H
#define SINGLEJUMPSCONFIGWINDOW_H

#include <QDialog>

class JumperEditorWidget;
class HillEditorWidget;
class WindsGeneratorSettingsEditorWidget;

namespace Ui {
class SingleJumpsConfigWindow;
}

class SingleJumpsConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SingleJumpsConfigWindow(QWidget *parent = nullptr);
    ~SingleJumpsConfigWindow();

    JumperEditorWidget *getJumperEditor() const;
    HillEditorWidget *getHillEditor() const;
    WindsGeneratorSettingsEditorWidget *getWindsGeneratorSettingsEditor() const;

    int getJumpsCountFromInput();
    bool getChangeableWindFromInput();
    bool getSaveResultsToFileFromInput();
    QString getResultsFileName();
    short getWindAverageCalculatingType();
    short getWindCompensationDistanceEffect();
    int getGateFromInput();

private slots:
    void on_comboBox_existingHill_currentIndexChanged(int index);
    void on_comboBox_existingJumper_currentIndexChanged(int index);
    void on_pushButton_submit_clicked();

    void on_checkBox_saveResultsFile_stateChanged(int arg1);

private:
    Ui::SingleJumpsConfigWindow *ui;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    WindsGeneratorSettingsEditorWidget * windsGeneratorSettingsEditor;
};

#endif // SINGLEJUMPSCONFIGWINDOW_H
