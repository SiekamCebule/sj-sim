#ifndef APPSETTINGSWINDOW_H
#define APPSETTINGSWINDOW_H

#include <QDialog>

class GlobalAppSettings;
class QCloseEvent;
class MainWindow;

namespace Ui {
class AppSettingsWindow;
}

class AppSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettingsWindow(QWidget *parent = nullptr);
    ~AppSettingsWindow();

    void setupLanguagesComboBox();

    MainWindow *getMainWindowParent() const;
    void setMainWindowParent(MainWindow *newMainWindowParent);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_pushButton_defaultSettings_clicked();

      void myConnect_on_comboBox_language_currentIndexChanged(int index);
    void on_spinBox_dsqProbability_valueChanged(int arg1);
    void on_spinBox_skillsRange_valueChanged(int arg1);

    void on_pushButton_repairDatabase_clicked();

private:
    Ui::AppSettingsWindow *ui;

    MainWindow * mainWindowParent;
};


#endif // APPSETTINGSWINDOW_H
