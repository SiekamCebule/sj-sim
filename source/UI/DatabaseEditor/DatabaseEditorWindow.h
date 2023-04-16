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
class JumpersListModel;
class HillsListModel;
class CompetitionRulesListModel;
class DatabaseItemsListView;

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

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void onJumpersListViewDoubleClicked(const QModelIndex &index);
    void onHillsListViewDoubleClicked(const QModelIndex &index);
    void onCompetitionRulesListViewDoubleClicked(const QModelIndex &index);
    //void on_listView_jumpers_doubleClicked(const QModelIndex &index);
    //void on_listView_hills_doubleClicked(const QModelIndex &index);
    //void on_listView_competitionRules_doubleClicked(const QModelIndex &index);
    //void on_pushButton_add_clicked();
    //void on_pushButton_remove_clicked();
    //void on_pushButton_up_clicked();
    //void on_pushButton_down_clicked();

private:
    QVector<Jumper> tempGlobalJumpers;
    QVector<Hill> tempGlobalHills;
    QVector<CompetitionRules> tempGlobalCompetitionRules;

    JumpersListModel * jumpersListModel;
    HillsListModel * hillsListModel;
    CompetitionRulesListModel * competiitonRulesListModel;

    DatabaseItemsListView * jumpersListView;
    DatabaseItemsListView * hillsListView;
    DatabaseItemsListView * competitionRulesListView;

    Ui::DatabaseEditorWindow *ui;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;
    CompetitionRulesEditorWidget * competitionRulesEditor;

    short actualElementType;
    int actualElementIndex;
public:
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
    JumpersListModel *getJumpersListModel() const;
    void setJumpersListModel(JumpersListModel *newJumpersListModel);
    int getActualElementIndex() const;
    void setActualElementIndex(int newActualElementIndex);
    HillsListModel *getHillsListModel() const;
    void setHillsListModel(HillsListModel *newHillsListModel);
    CompetitionRulesListModel *getCompetitionRulesEditor() const;
    void setCompetitionRulesEditor(CompetitionRulesListModel *newCompetitionRulesEditor);
    CompetitionRulesListModel *getCompetitionRulesListModel() const;
    void setCompetitionRulesListModel(CompetitionRulesListModel *newCompetitionRulesListModel);
};

#endif // DATABASEEDITORWINDOW_H
