#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QCloseEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setParentApplication(QApplication *newParentApplication);
    QApplication *getParentApplication() const;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_singleJumps_clicked();
    void on_pushButton_databaseEdit_clicked();
    void on_pushButton_settings_clicked();

private:
    Ui::MainWindow *ui;

    QApplication * parentApplication;
};
#endif // MAINWINDOW_H
