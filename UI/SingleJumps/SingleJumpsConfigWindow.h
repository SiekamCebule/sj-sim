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

private slots:
    void on_comboBox_existingHill_currentIndexChanged(int index);
    void on_comboBox_existingJumper_currentIndexChanged(int index);
    void on_pushButton_submit_clicked();

private:
    Ui::SingleJumpsConfigWindow *ui;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    WindsGeneratorSettingsEditorWidget * windsGeneratorSettingsEditor;

public:
    int getJumpsCountFromInput();
    bool getChangeableWindFromInput();
    short getResultsFormatFromInput();
    QString getResultsFileName();
    short getWindAverageCalculatingType();
    short getWindCompensationDistanceEffect();
    int getGateFromInput();
};

#endif // SINGLEJUMPSCONFIGWINDOW_H
