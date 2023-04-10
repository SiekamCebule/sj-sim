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
class GlobalJumpersListModel;
class GlobalHillsListModel;
class GlobalCompetitionRulesListModel;

namespace Ui {
class DatabaseEditorWindow;
}

class DatabaseEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEditorWindow(JumperEditorWidget * jumperEditor = nullptr, HillEditorWidget * hillEditor = nullptr, CompetitionRulesEditorWidget * competitionRulesEditor = nullptr, QWidget *parent = nullptr);
    ~DatabaseEditorWindow();

    void replaceJumperByJumperEditor();
    void replaceHillByHillEditor();
    void replaceCompetitionRuleByCompetitionRulesEditor();

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
    GlobalJumpersListModel *getJumpersListModel() const;
    void setJumpersListModel(GlobalJumpersListModel *newJumpersListModel);
    int getActualElementIndex() const;
    void setActualElementIndex(int newActualElementIndex);
    GlobalHillsListModel *getHillsListModel() const;
    void setHillsListModel(GlobalHillsListModel *newHillsListModel);
    GlobalCompetitionRulesListModel *getCompetitionRulesEditor() const;
    void setCompetitionRulesEditor(GlobalCompetitionRulesListModel *newCompetitionRulesEditor);

    GlobalCompetitionRulesListModel *getCompetitionRulesListModel() const;
    void setCompetitionRulesListModel(GlobalCompetitionRulesListModel *newCompetitionRulesListModel);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_listView_jumpers_doubleClicked(const QModelIndex &index);
    void on_listView_hills_doubleClicked(const QModelIndex &index);
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

    void on_listView_competitionRules_doubleClicked(const QModelIndex &index);

private:
    QVector<Jumper> tempGlobalJumpers;
    QVector<Hill> tempGlobalHills;
    QVector<CompetitionRules> tempGlobalCompetitionRules;

    GlobalJumpersListModel * jumpersListModel;
    GlobalHillsListModel * hillsListModel;
    GlobalCompetitionRulesListModel * competitionRulesListModel;

    Ui::DatabaseEditorWindow *ui;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    CompetitionRulesEditorWidget * competitionRulesEditor;

    short actualElementType;
    int actualElementIndex;
};

#endif // DATABASEEDITORWINDOW_H
