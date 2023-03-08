#ifndef DATABASEEDITORWINDOW_H
#define DATABASEEDITORWINDOW_H

#include <QDialog>
#include <QVector>
#include <QEvent>

class QCloseEvent;

class HillEditorWidget;
class JumperEditorWidget;
class CompetitionRulesEditorWidget;
class Jumper;
class Hill;
class DatabaseListItemWidget;
class CompetitionRules;

namespace Ui {
class DatabaseEditorWindow;
}

class DatabaseEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEditorWindow(JumperEditorWidget * jumperEditor = nullptr, HillEditorWidget * hillEditor = nullptr, CompetitionRulesEditorWidget * competitionRulesEditor = nullptr, QWidget *parent = nullptr);
    ~DatabaseEditorWindow();

    void when_ItemWidgetDeleteButtonClicked(int index);
    void updateItemsSelection(int index);

    enum ElementType{
        JumperElement,
        HillElement,
        CompetitionRulesElement
    };

    short getActualElementType() const;
    void setActualElementType(short newActualElementType);
    int getSelectedItemIndex() const;
    void setSelectedItemIndex(int newSelectedItemIndex);
    QVector<Jumper> getTempGlobalJumpers() const;
    void setTempGlobalJumpers(const QVector<Jumper> &newTempGlobalJumpers);
    QVector<Hill> getTempGlobalHills() const;
    void setTempGlobalHills(const QVector<Hill> &newTempGlobalHills);
    QVector<CompetitionRules> getTempGlobalCompetitionRules() const;
    void setTempGlobalCompetitionRules(const QVector<CompetitionRules> &newTempGlobalCompetitionRules);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

    void replaceJumperFromJumperEditor();
    void replaceHillFromHillEditor();
    void replaceCompetitionRulesFromEditor();

private:
    QVector<Jumper> tempGlobalJumpers;
    QVector<Hill> tempGlobalHills;
    QVector<CompetitionRules> tempGlobalCompetitionRules;

    Ui::DatabaseEditorWindow *ui;
    QVector<DatabaseListItemWidget *> jumpersListItems;
    QVector<DatabaseListItemWidget *> hillsListItems;
    QVector<DatabaseListItemWidget *> competitionRulesListItems;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    CompetitionRulesEditorWidget * competitionRulesEditor;

    void fillJumpersWidget();
    void fillHillsWidget();
    void fillCompetitionRulesWidget();
    short actualElementType;
    int selectedItemIndex;

    void updateIndexes();
};

#endif // DATABASEEDITORWINDOW_H
