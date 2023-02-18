#include "SingleJumpsConfigWindow.h"
#include "ui_SingleJumpsConfigWindow.h"

SingleJumpsConfigWindow::SingleJumpsConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsConfigWindow)
{
    ui->setupUi(this);
}

SingleJumpsConfigWindow::~SingleJumpsConfigWindow()
{
    delete ui;
}
