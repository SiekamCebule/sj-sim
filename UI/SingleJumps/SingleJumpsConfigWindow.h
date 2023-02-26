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

private slots:
    void on_comboBox_existingHill_currentIndexChanged(int index);
    void on_comboBox_existingJumper_currentIndexChanged(int index);

    void on_pushButton_submit_clicked();

private:
    Ui::SingleJumpsConfigWindow *ui;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    WindsGeneratorSettingsEditorWidget * windsGeneratorSettingsEditor;

};

#endif // SINGLEJUMPSCONFIGWINDOW_H
