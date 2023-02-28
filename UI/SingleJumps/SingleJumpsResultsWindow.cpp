#include "SingleJumpsResultsWindow.h"
#include "ui_SingleJumpsResultsWindow.h"

#include "SingleJumpMiniResultWidget.h"

SingleJumpsResultsWindow::SingleJumpsResultsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsResultsWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);
}

SingleJumpsResultsWindow::~SingleJumpsResultsWindow()
{
    delete ui;
}

void SingleJumpsResultsWindow::fillMiniJumpsResultsLayout()
{
    int i=1;
    for(const auto & jump : manager->getJumps())
    {
        SingleJumpMiniResultWidget * widget = new SingleJumpMiniResultWidget;
        widget->setJumpData(const_cast<JumpData*>(&jump));
        widget->setIndexInList(i);
        widget->fillUi();

        ui->verticalLayout_singleJumpResultWidgets->addWidget(widget);

        i++;
    }
    ui->verticalLayout_singleJumpResultWidgets->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

SingleJumpsManager *SingleJumpsResultsWindow::getManager() const
{
    return manager;
}

void SingleJumpsResultsWindow::setManager(SingleJumpsManager *newManager)
{
    manager = newManager;
}
