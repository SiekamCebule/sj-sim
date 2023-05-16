#include "SeasonCalendarEditor.h"
#include "ui_SeasonCalendarEditor.h"

SeasonCalendarEditor::SeasonCalendarEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SeasonCalendarEditor)
{
    ui->setupUi(this);
}

SeasonCalendarEditor::~SeasonCalendarEditor()
{
    delete ui;
}
