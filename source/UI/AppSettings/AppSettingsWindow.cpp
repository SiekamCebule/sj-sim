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
    ui->doubleSpinBox_randomMultiplier->setValue(GlobalSimulationSettings::get()->getSimulationRandomMultiplier());
    ui->checkBox_autoAdjustHillEffects->setChecked(GlobalSimulationSettings::get()->getAutoAdjustHillEffects());
    ui->doubleSpinBox->setValue(GlobalSimulationSettings::get()->getHillsEffectsMultiplier());
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
    ui->spinBox_dsqProbability->setValue(150);
    ui->spinBox_skillsRange->setValue(100);
    ui->doubleSpinBox_randomMultiplier->setValue(1);
    ui->checkBox_autoAdjustHillEffects->setChecked(false);
    ui->doubleSpinBox->setValue(1);
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
    GlobalDatabase * db = GlobalDatabase::get();
    /*if(db->getLoadedSimulationSaves() == false){
        db->loadSimulationSaves(true);
        db->setLoadedSimulationSaves(true);
    }*/
    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(db->getEditableGlobalJumpers().count() +  db->getEditableGlobalHills().count()
                      + db->getEditableCompetitionRules().count() + db->getEditableGlobalSimulationSaves().count() + 1);
    dialog.setMinimumDuration(0);
    dialog.setValue(0);
    dialog.setWindowTitle(QObject::tr("Naprawa bazy danych"));
    //dialog.setLabelText(QString(QObject::tr("Postęp naprawy bazy danych: %1 z %2")).arg(QString::number(dialog.value())).arg(QString::number(dialog.maximum())));
    dialog.setModal(true);
    dialog.setWindowModality(Qt::WindowModal);

    QSet<ulong> values; //Te ktore już wystąpiły

    for(auto & jumper : db->getEditableGlobalJumpers()){
        if(values.contains(jumper.getID()))
            jumper.generateID();
        else jumper.regenerateID();
        values.insert(jumper.getID());
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
    }
    for(auto & hill : db->getEditableGlobalHills()){
        if(values.contains(hill.getID()))
            hill.generateID();
        else hill.regenerateID();
        values.insert(hill.getID());
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
    }
    for(auto & rules : db->getEditableCompetitionRules()){
        if(values.contains(rules.getID()))
            rules.generateID();
        else rules.regenerateID();
        values.insert(rules.getID());
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
    }
    for(auto & calendarPreset : db->getEditableGlobalCalendarPresets())
    {
            for(auto & competition : calendarPreset.getCalendarReference().getCompetitionsReference())
            {
                if(values.contains(competition->getID()))
                    competition->generateID();
                else competition->regenerateID();

                if(values.contains(competition->getResultsReference().getID()))
                    competition->getResultsReference().generateID();
                else competition->getResultsReference().regenerateID();

                for(auto & groups : competition->getRoundsKOGroupsReference())
                {
                    for(auto & group : groups){
                        if(values.contains(group.getID()))
                            group.generateID();
                        else group.regenerateID();
                    }
                }
                for(auto & team : competition->getTeamsReference())
                {
                    if(values.contains(team.getID()))
                        team.generateID();
                    else team.regenerateID();
                }
            }
            for(auto & classification : calendarPreset.getCalendarReference().getClassificationsReference())
            {
                if(values.contains(classification->getID()))
                    classification->generateID();
                else classification->regenerateID();
            }
    }
    /*for(auto & save : db->getEditableGlobalSimulationSaves()){
        if(values.contains(save->getID()))
            save->generateID();
        else save->regenerateID();
        values.insert(save->getID());
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
    }*/

    db->writeToJson();
    dialog.setValue(dialog.value() + 1);

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
    GlobalDatabase * db = GlobalDatabase::get();
    if(db->getLoadedSimulationSaves() == false){
        db->loadSimulationSaves(true);
        db->setLoadedSimulationSaves(true);
    }

    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(db->getEditableGlobalSimulationSaves().count() * 11);
    dialog.setMinimumDuration(0);
    dialog.setValue(0);
    dialog.setWindowTitle(QObject::tr("Naprawa zapisów symulacji "));
    //dialog.setLabelText(QString(QObject::tr("Postęp naprawiania zapisów symulacji: %1 z %2")).arg(QString::number(dialog.value())).arg(QString::number(dialog.maximum())));
    dialog.setWindowModality(Qt::WindowModal);

    for(auto & save : GlobalDatabase::get()->getEditableGlobalSimulationSaves())
    {
        SeasonCalendar * calendar = &save->getActualSeason()->getCalendarReference();
        calendar->fixAdvancementClassifications();
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        calendar->fixAdvancementCompetitions();
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        calendar->fixCompetitionsClassifications();
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        calendar->fixCompetitionsHills(&save->getHillsReference(), save->getHillsReference().first()  );
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        save->repairDatabase();
        dialog.setValue(dialog.value() + 5);
        QCoreApplication::processEvents();
        save->saveToFile("simulationSaves/");
        dialog.setValue(dialog.value() + 2);
        QCoreApplication::processEvents();
    }
    QMessageBox::information(this, tr("Naprawiono zapisy symulacji"), tr("Naprawiono zapisy symulacji. Aby zmiana weszła w życie wejdź jeszcze raz do programu aby ponownie wczytać zapisy symulacji."), QMessageBox::Ok);
}


void AppSettingsWindow::on_doubleSpinBox_randomMultiplier_valueChanged(double arg1)
{
    GlobalSimulationSettings::get()->setSimulationRandomMultiplier(arg1);
}

void AppSettingsWindow::on_checkBox_autoAdjustHillEffects_stateChanged(int arg1)
{
    if(arg1 == 0)
         GlobalSimulationSettings::get()->setAutoAdjustHillEffects(false);
    else if(arg1 == 2)
        GlobalSimulationSettings::get()->setAutoAdjustHillEffects(true);
}


void AppSettingsWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    GlobalSimulationSettings::get()->setHillsEffectsMultiplier(arg1);
}

