#ifndef TEAMEDITORWIDGET_H
#define TEAMEDITORWIDGET_H

#include <QWidget>
#include <QVector>
#include <QStandardItemModel>
#include <QStandardItem>
#include "../../../simulator/Team.h"
#include "../../../simulator/Jumper.h"

namespace Ui {
class TeamEditorWidget;
}

class TeamEditorWidget : public QWidget
{
    Q_OBJECT

public:
    TeamEditorWidget(const QString &teamCode = "", const QVector<Jumper *> &jumpers = QVector<Jumper *>(), QWidget * parent = nullptr);
    ~TeamEditorWidget();

    void fillWidgetInputs();
    void setupItemsList();
    void removeSubmitButton();
    Team constructTeamFromWidgetInput();

signals:
    void submitted();
    void needToUpdateModel();

private:
    Ui::TeamEditorWidget *ui;
    QVector<QStandardItem *> itemsList;
    QStandardItemModel * model;

    QString teamCode;

    QVector<Jumper *> jumpers;
    int jumpersCount;

private slots:
    void when_submitButtonClicked();
    void updateModelByItemsListVector();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

public:
    QString getTeamCode() const;
    void setTeamCode(const QString &newTeamCode);
    int getJumpersCount() const;
    void setJumpersCount(int newJumpersCount);
};

#endif // TEAMEDITORWIDGET_H
