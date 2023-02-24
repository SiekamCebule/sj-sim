#ifndef DATABASEEDITORWINDOW_H
#define DATABASEEDITORWINDOW_H

#include <QDialog>
#include <QVector>
#include <QEvent>

class QCloseEvent;

class HillEditorWidget;
class JumperEditorWidget;
class Jumper;
class Hill;
class DatabaseListItemWidget;

namespace Ui {
class DatabaseEditorWindow;
}

class DatabaseEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEditorWindow(JumperEditorWidget * jumperEditor = nullptr, HillEditorWidget * hillEditor = nullptr, QWidget *parent = nullptr);
    ~DatabaseEditorWindow();

    void when_ItemWidgetDeleteButtonClicked(int index);
    void updateItemsSelection(int index);

    enum ElementType{
        JumperElement,
        HillElement,
        WindGeneratorElement
    };

    short getActualElementType() const;
    void setActualElementType(short newActualElementType);
    int getSelectedItemIndex() const;
    void setSelectedItemIndex(int newSelectedItemIndex);
    QVector<Jumper> getTempGlobalJumpers() const;
    void setTempGlobalJumpers(const QVector<Jumper> &newTempGlobalJumpers);
    QVector<Hill> getTempGlobalHills() const;
    void setTempGlobalHills(const QVector<Hill> &newTempGlobalHills);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

    void replaceJumperFromJumperEditor();
    void replaceHillFromHillEditor();

private:
    QVector<Jumper> tempGlobalJumpers;
    QVector<Hill> tempGlobalHills;

    Ui::DatabaseEditorWindow *ui;
    QVector<DatabaseListItemWidget *> jumpersListItems;
    QVector<DatabaseListItemWidget *> hillsListItems;

    JumperEditorWidget * jumperEditor;
    HillEditorWidget * hillEditor;

    void fillJumpersWidget();
    void fillHillsWidget();
    short actualElementType;
    int selectedItemIndex;

    void updateIndexes();
};

#endif // DATABASEEDITORWINDOW_H
