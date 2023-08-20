#include "AppSettingsWindow.h"
#include "ui_AppSettingsWindow.h"

#include <QTranslator>
#include <QCloseEvent>
#include <QProgressDialog>
#include <QMessageBox>

#include "../mainwindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../global/GlobalTranslators.h"
#include "../../global/CountryFlagsManager.h"
#include "../../global/GlobalSimulationSettings.h"
#include "../../global/GlobalDatabase.h"
#include "../../global/IDGenerator.h"

extern IDGenerator globalIDGenerator;

AppSettingsWindow::AppSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowFlags(Qt::Window);

    ui->spinBox_skillsRange->setValue(GlobalSimulationSettings::get()->getMaxSkills());
    ui->spinBox_dsqProbability->setValue(GlobalSimulationSettings::get()->getBaseDsqProbability());
}

AppSettingsWindow::~AppSettingsWindow()
{
    delete ui;
}

void AppSettingsWindow::setupLanguagesComboBox()
{
    ui->comboBox_language->addItem(QIcon(CountryFlagsManager::getFlagPixmap("pl")), QObject::tr("Polski"));
    ui->comboBox_language->addItem(QIcon(CountryFlagsManager::getFlagPixmap("gb")), QObject::tr("Angielski"));

    ui->comboBox_language->setCurrentIndex(GlobalAppSettings::get()->getLanguageID());
    connect(ui->comboBox_language, &QComboBox::currentIndexChanged, this, &AppSettingsWindow::myConnect_on_comboBox_language_currentIndexChanged);
}

void AppSettingsWindow::on_pushButton_defaultSettings_clicked()
{
    ui->comboBox_language->setCurrentIndex(0);
}

MainWindow *AppSettingsWindow::getMainWindowParent() const
{
    return mainWindowParent;
}

void AppSettingsWindow::setMainWindowParent(MainWindow *newMainWindowParent)
{
    mainWindowParent = newMainWindowParent;
}

void AppSettingsWindow::closeEvent(QCloseEvent *event)
{
    GlobalSimulationSettings::get()->updateSimulationRandomMultiplier();
    GlobalSimulationSettings::get()->writeToFile();
    event->accept();
}

void AppSettingsWindow::myConnect_on_comboBox_language_currentIndexChanged(int index)
{
    switch(index)
    {
    case GlobalAppSettings::Polish:
        dynamic_cast<MainWindow *>(getMainWindowParent())->getParentApplication()->removeTranslator(const_cast<QTranslator *>(GlobalTranslators::get()->getGlobalTranslations().at(0)));
        ui->retranslateUi(this);
        GlobalAppSettings::get()->setLanguageID(GlobalAppSettings::Polish);
        break;
    case GlobalAppSettings::English:
        dynamic_cast<MainWindow *>(getMainWindowParent())->getParentApplication()->installTranslator(const_cast<QTranslator *>(GlobalTranslators::get()->getGlobalTranslations().at(0)));
        ui->retranslateUi(this);
        GlobalAppSettings::get()->setLanguageID(GlobalAppSettings::English);
        break;
    }
}

void AppSettingsWindow::on_spinBox_dsqProbability_valueChanged(int arg1)
{
    GlobalSimulationSettings::get()->setBaseDsqProbability(arg1);
}

void AppSettingsWindow::on_spinBox_skillsRange_valueChanged(int arg1)
{
    GlobalSimulationSettings::get()->setMaxSkills(arg1);
}


void AppSettingsWindow::on_pushButton_repairDatabase_clicked()
{
    QProgressDialog * dialog = new QProgressDialog(this);
    dialog->setLabelText("Naprawianie bazy danych...");
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog->setMinimum(0);
    dialog->setMaximum(6);
    dialog->setWindowTitle(QObject::tr("Naprawa bazy danych"));
    dialog->setValue(0);

    GlobalDatabase * db = GlobalDatabase::get();
    dialog->setValue(1);

    QSet<ulong> values; //Te ktore już wystąpiły

    for(auto & jumper : db->getEditableGlobalJumpers()){
        if(values.contains(jumper.getID()))
            jumper.generateID();
        else jumper.regenerateID();
        values.insert(jumper.getID());
    }
    dialog->setValue(2);
    for(auto & hill : db->getEditableGlobalHills()){
        if(values.contains(hill.getID()))
            hill.generateID();
        else hill.regenerateID();
        values.insert(hill.getID());
    }
    dialog->setValue(3);
    for(auto & rules : db->getEditableCompetitionRules()){
        if(values.contains(rules.getID()))
            rules.generateID();
        else rules.regenerateID();
        values.insert(rules.getID());
    }
    dialog->setValue(4);
    for(auto & save : db->getEditableGlobalSimulationSaves()){
        if(values.contains(save->getID()))
            save->generateID();
        else save->regenerateID();
        values.insert(save->getID());
    }
    dialog->setValue(5);

    db->writeToJson();
    dialog->setValue(6);

    QMessageBox::information(this, tr("Naprawa bazy danych"), tr("Naprawiono bazę danych"), QMessageBox::Ok);
}


void AppSettingsWindow::on_pushButton_cutSurnames_clicked()
{
    GlobalDatabase * db = GlobalDatabase::get();
    for(auto & jumper : db->getEditableGlobalJumpers())
    {
        QStringList words = jumper.getSurname().split(' ');
        jumper.setSurname("");
        if(words.count() > 0)
            for(auto & word : words){
                jumper.setSurname(jumper.getSurname() + word[0] + " ");
                QString surname = jumper.getSurname();
                surname.chop(1);
                jumper.setSurname(surname);
        }
    }
    db->writeJumpers();
    QMessageBox::information(this, tr("Nazwiska zawodników"), tr("Pomyślnie skrócono nazwiska zawodników"), QMessageBox::Ok);
}


void AppSettingsWindow::on_pushButton_repairDatabase_2_clicked()
{
    QProgressDialog * dialog = new QProgressDialog(this);
    dialog->setLabelText("Naprawianie zapisów symulacji...");
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog->setMinimum(0);
    dialog->setMaximum(11 * GlobalDatabase::get()->getEditableGlobalSimulationSaves().count());
    dialog->setWindowTitle(QObject::tr("Naprawa zapisów symulacji."));
    dialog->setValue(0);

    for(auto & save : GlobalDatabase::get()->getEditableGlobalSimulationSaves())
    {
        SeasonCalendar * calendar = &save->getActualSeason()->getCalendarReference();
        calendar->fixAdvancementClassifications();
        dialog->setValue(dialog->value() + 1);
        calendar->fixAdvancementCompetitions();
        dialog->setValue(dialog->value() + 1);
        calendar->fixCompetitionsClassifications();
        dialog->setValue(dialog->value() + 1);
        calendar->fixCompetitionsHills(&save->getHillsReference());
        dialog->setValue(dialog->value() + 1);
        save->repairDatabase();
        dialog->setValue(dialog->value() + 5);
        save->saveToFile("simulationSaves/");
        dialog->setValue(dialog->value() + 2);
    }
    QMessageBox::information(this, tr("Naprawiono zapisy symulacji"), tr("Naprawiono zapisy symulacji. Aby zmiana weszła w życie wejdź jeszcze raz do programu aby ponownie wczytać zapisy symulacji."), QMessageBox::Ok);
}

