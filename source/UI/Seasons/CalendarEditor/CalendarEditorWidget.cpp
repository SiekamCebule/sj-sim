#include "CalendarEditorWidget.h"
#include "ui_CalendarEditorWidget.h"

CalendarEditorWidget::CalendarEditorWidget(CalendarEditorTreeModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalendarEditorWidget),
    model(model)
{
    ui->setupUi(this);
}

CalendarEditorWidget::~CalendarEditorWidget()
{
    delete ui;
}
