#ifndef NEWSEASONCONFIGURATORWINDOW_H
#define NEWSEASONCONFIGURATORWINDOW_H

#include <QDialog>
#include <QAction>
#include "../DatabaseEditor/ListModels/JumpersListModel.h"
class DatabaseItemsListView;

namespace Ui {
class NewSeasonConfiguratorDialog;
}

class NewSeasonConfiguratorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewSeasonConfiguratorWindow(QWidget *parent = nullptr);
    ~NewSeasonConfiguratorWindow();

private slots:
    void on_pushButton_submit_clicked();

private:
    Ui::NewSeasonConfiguratorDialog *ui;

    QVector<Jumper> jumpers;
    DatabaseItemsListView * jumpersListView;

public:
    QVector<Jumper> getJumpers() const;
    QVector<Jumper>& getJumpersReference();
    void setJumpers(const QVector<Jumper> &newJumpers);
};

#endif // NEWSEASONCONFIGURATORWINDOW_H
