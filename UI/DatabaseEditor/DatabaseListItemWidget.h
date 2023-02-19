#ifndef DATABASELISTITEMWIDGET_H
#define DATABASELISTITEMWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QMouseEvent>

class DatabaseEditorWindow;

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

    bool getIsSelected() const;
    void setIsSelected(bool newIsSelected);

signals:
    void itemSelected(int index);

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    Ui::DatabaseListItemWidget *ui;
    QVector<QLabel *> labels;

    int index;
    bool isSelected;

    DatabaseEditorWindow * editorParent;
};

#endif // DATABASELISTITEMWIDGET_H
