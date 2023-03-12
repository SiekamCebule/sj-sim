#include "CompetitionConfigWindow.h"
#include "ui_SingleCompetitionConfigWindow.h"

#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "../EditorWidgets/OtherCompetitionSettingsWidget.h"
#include "StartList/CompetitionStartListDisplayWidget.h"
#include "../../global/GlobalDatabase.h"

#include <QSizePolicy>

CompetitionConfigWindow::CompetitionConfigWindow(short type, QWidget *parent) :
    type(type),
    QDialog(parent),
    ui(new Ui::SingleCompetitionConfigWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    delete ui->page;
    delete ui->page_2;
    ui->toolBox->removeItem(0);
    ui->toolBox->removeItem(0);

    hillEditor = new HillEditorWidget;
    hillEditor->removeSubmitButton();
    windGeneratorSettingsEditor = new WindsGeneratorSettingsEditorWidget;
    windGeneratorSettingsEditor->setRemovingSubmitButtons(true);
    competitionRulesEditor = new CompetitionRulesEditorWidget;
    otherCompetitionSettingsEditor = new OtherCompetitionSettingsWidget;
    competitionRulesEditor->setCompetitionRules(new CompetitionRules(tr("Zasady")));
    competitionRulesEditor->removeSubmitButton();
    startListDisplayWidget = new CompetitionStartListDisplayWidget;
    startListDisplayWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    ui->toolBox->addItem(hillEditor, "Skocznia");
    ui->toolBox->addItem(windGeneratorSettingsEditor, "Ustawienia generatora wiatru");
    ui->toolBox->addItem(competitionRulesEditor, "Zasady konkursu");
    ui->toolBox->addItem(otherCompetitionSettingsEditor, "Inne opcje");
    ui->verticalLayout_startList->addWidget(startListDisplayWidget);
    ui->verticalLayout_startList->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    if(getType() == SeasonCompetition)
    {
        setWindowTitle("Konfiguracja konkursu");
        delete ui->toolBox->widget(0); //index 0
        ui->toolBox->removeItem(0);
        delete ui->toolBox->widget(1); //index 2
        ui->toolBox->removeItem(1);
    }
    else if(getType() == SingleCompetition)
    {
        setWindowTitle("Konfiguracja pojedynczego konkursu");
        competitionJumpers = GlobalDatabase::get()->getGlobalJumpers();
        startListDisplayWidget->setJumpers(&competitionJumpers);
        startListDisplayWidget->fillCompetitiorsActivity();
        startListDisplayWidget->fillItemsLayout();
    }

    connect(hillEditor, &HillEditorWidget::KPointInputChanged, windGeneratorSettingsEditor, &WindsGeneratorSettingsEditorWidget::fillSettingsInputs);
}

CompetitionConfigWindow::~CompetitionConfigWindow()
{
    delete ui;
}

QVector<Jumper> CompetitionConfigWindow::getCompetitionJumpers() const
{
    return competitionJumpers;
}

CompetitionStartListDisplayWidget *CompetitionConfigWindow::getStartListDisplayWidget() const
{
    return startListDisplayWidget;
}

void CompetitionConfigWindow::setStartListDisplayWidget(CompetitionStartListDisplayWidget *newStartListDisplayWidget)
{
    startListDisplayWidget = newStartListDisplayWidget;
}

OtherCompetitionSettingsWidget *CompetitionConfigWindow::getOtherCompetitionSettingsEditor() const
{
    return otherCompetitionSettingsEditor;
}

void CompetitionConfigWindow::setOtherCompetitionSettingsEditor(OtherCompetitionSettingsWidget *newOtherCompetitionSettingsEditor)
{
    otherCompetitionSettingsEditor = newOtherCompetitionSettingsEditor;
}

HillEditorWidget *CompetitionConfigWindow::getHillEditor() const
{
    return hillEditor;
}

void CompetitionConfigWindow::setHillEditor(HillEditorWidget *newHillEditor)
{
    hillEditor = newHillEditor;
}

CompetitionRulesEditorWidget *CompetitionConfigWindow::getCompetitionRulesEditor() const
{
    return competitionRulesEditor;
}

void CompetitionConfigWindow::setCompetitionRulesEditor(CompetitionRulesEditorWidget *newCompetitionRulesEditor)
{
    competitionRulesEditor = newCompetitionRulesEditor;
}

WindsGeneratorSettingsEditorWidget *CompetitionConfigWindow::getWindGeneratorSettingsEditor() const
{
    return windGeneratorSettingsEditor;
}

void CompetitionConfigWindow::setWindGeneratorSettingsEditor(WindsGeneratorSettingsEditorWidget *newWindsGeneratorSettingsEditor)
{
    windGeneratorSettingsEditor = newWindsGeneratorSettingsEditor;
}

short CompetitionConfigWindow::getType() const
{
    return type;
}

void CompetitionConfigWindow::setType(short newType)
{
    type = newType;
}
