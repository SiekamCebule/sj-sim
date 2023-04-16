#include "NewSeasonConfiguratorWindow.h"
#include "ui_NewSeasonConfiguratorWindow.h"

#include "../../global/GlobalDatabase.h"

NewSeasonConfiguratorWindow::NewSeasonConfiguratorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSeasonConfiguratorDialog)
{
    ui->setupUi(this);
    jumpers = GlobalDatabase::get()->getEditableGlobalJumpers();
    jumpersListModel = new JumpersListModel(&this->jumpers, this);
    ui->listView_jumpers->setModel(jumpersListModel);
}

NewSeasonConfiguratorWindow::~NewSeasonConfiguratorWindow()
{
    delete ui;
}

void NewSeasonConfiguratorWindow::on_pushButton_submit_clicked()
{
    accept();
}

QVector<Jumper> NewSeasonConfiguratorWindow::getJumpers() const
{
    return jumpers;
}

QVector<Jumper> &NewSeasonConfiguratorWindow::getJumpersReference()
{
    return jumpers;
}

void NewSeasonConfiguratorWindow::setJumpers(const QVector<Jumper> &newJumpers)
{
    jumpers = newJumpers;
}

