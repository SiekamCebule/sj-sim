#ifndef DATABASEEDITORWINDOW_H
#define DATABASEEDITORWINDOW_H

#include <QDialog>
#include <QVector>

class QCloseEvent;

class JumperEditorWidget;
class Jumper;
class DatabaseListItemWidget;

namespace Ui {
class DatabaseEditorWindow;
}

class DatabaseEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEditorWindow(JumperEditorWidget * jumperEditor = nullptr, QWidget *parent = nullptr);
    ~DatabaseEditorWindow();

    void when_ItemWidgetDeleteButtonClicked(int index);
    void updateItemsSelection(int index);

    enum class ElementType{
        Jumper,
        Hill,
        WindGenerator
    };

    short getActualElementType() const;
    void setActualElementType(short newActualElementType);
    int getSelectedItemIndex() const;
    void setSelectedItemIndex(int newSelectedItemIndex);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

    void replaceJumperFromJumperEdit();

private:
    Ui::DatabaseEditorWindow *ui;
    QVector<DatabaseListItemWidget *> listItems;

    JumperEditorWidget * jumperEditor;

    void fillJumpersWidget();
    void updateOneJumperWidget(int indexFromZero);
    short actualElementType;
    int selectedItemIndex;

    void updateIndexes();
};

#endif // DATABASEEDITORWINDOW_H
