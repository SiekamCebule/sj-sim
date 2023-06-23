#ifndef CALENDAREDITORWIDGET_H
#define CALENDAREDITORWIDGET_H

#include <QWidget>
#include "CalendarEditorTreeModel.h"

namespace Ui {
class CalendarEditorWidget;
}

class CalendarEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarEditorWidget(CalendarEditorTreeModel * model, QWidget *parent = nullptr);
    ~CalendarEditorWidget();

private:
    Ui::CalendarEditorWidget *ui;
    CalendarEditorTreeModel * model;
};

#endif // CALENDAREDITORWIDGET_H
