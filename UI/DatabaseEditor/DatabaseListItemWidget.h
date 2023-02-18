#ifndef DATABASELISTITEMWIDGET_H
#define DATABASELISTITEMWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLabel>

#include "DatabaseEditorWindow.h"

namespace Ui {
class DatabaseListItemWidget;
}

class DatabaseListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseListItemWidget(QWidget *parent = nullptr);
    ~DatabaseListItemWidget();

    QVector<QLabel *> getLabels() const;
    void addLabel(QLabel * label, int index = 0);
    void update();

    int getIndex() const;
    void setIndex(int newIndex);

    DatabaseEditorWindow *getEditorParent() const;
    void setEditorParent(DatabaseEditorWindow *newEditorParent);

private:
    Ui::DatabaseListItemWidget *ui;
    QVector<QLabel *> labels;

    int index;

    DatabaseEditorWindow * editorParent;
};

#endif // DATABASELISTITEMWIDGET_H
