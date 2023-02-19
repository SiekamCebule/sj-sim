#ifndef DATABASEEDITORWINDOW_H
#define DATABASEEDITORWINDOW_H

#include <QDialog>
#include <QVector>

class DatabaseListItemWidget;

namespace Ui {
class DatabaseEditorWindow;
}

class DatabaseEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseEditorWindow(QWidget *parent = nullptr);
    ~DatabaseEditorWindow();

    void when_ItemWidgetDeleteButtonClicked(int index);
    void updateItemsSelection(int index);

private:
    Ui::DatabaseEditorWindow *ui;
    QVector<DatabaseListItemWidget *> listItems;

    void fillJumpersWidget();
};

#endif // DATABASEEDITORWINDOW_H
