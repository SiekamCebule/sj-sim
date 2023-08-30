#include "JumpersFormGeneratorConfigWindow.h"
#include "ui_JumpersFormGeneratorConfigWindow.h"
#include <QMessageBox>

JumpersFormGeneratorConfigWindow::JumpersFormGeneratorConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpersFormGeneratorConfigWindow)
{
    ui->setupUi(this);

    tableModel = new JumperFormGeneratorsSettingsTableModel(this);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

JumpersFormGeneratorConfigWindow::~JumpersFormGeneratorConfigWindow()
{
    delete ui;
}

void JumpersFormGeneratorConfigWindow::updateTable()
{
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(tableModel);
    ui->tableView->resizeColumnsToContents();
}

JumperFormGeneratorsSettingsTableModel *JumpersFormGeneratorConfigWindow::getTableModel() const
{
    return tableModel;
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_tendenceVariability_editingFinished()
{
    double value = ui->doubleSpinBox_tendenceVariability->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setTendenceVariability(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_tendBonus_editingFinished()
{
    double value = ui->doubleSpinBox_tendBonus->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setTendenceBonus(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_tendAlignment_editingFinished()
{
    double value = ui->doubleSpinBox_tendAlignment->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setTendenceAlignmentMultiplier(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_minTend_editingFinished()
{
    double value = ui->doubleSpinBox_minTend->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setMinTendence(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_maxTend_editingFinished()
{
    double value = ui->doubleSpinBox_maxTend->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setMaxTendence(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_tendVariability_editingFinished()
{
    double value = ui->doubleSpinBox_tendVariability->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setFormVariability(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_formBonus_editingFinished()
{
    double value = ui->doubleSpinBox_formBonus->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setFormBonus(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_minForm_editingFinished()
{
    double value = ui->doubleSpinBox_minForm->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setMinForm(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_doubleSpinBox_maxForm_editingFinished()
{
    double value = ui->doubleSpinBox_maxForm->value();
    for(auto & settings : tableModel->getGeneratorsSettingsReference())
    {
        settings.setMaxForm(value);
    }
    updateTable();
}

void JumpersFormGeneratorConfigWindow::on_listWidget_questions_doubleClicked(const QModelIndex &index)
{
    if(index.row() == 0)
    {
        QMessageBox::information(this, tr("Po co jest generator formy?"), tr(
        "Generator formy jest narzędziem służącym do pół-automatycznej zmiany formy zawodników pomiędzy okresem czasu (między zawodami, weekendami zawodów, sezonami).\nPosiada on wiele opcji, które służą do możliwie jak najwierniejszego odwzorowaniu zmiany formy prawdziwych skoczków między zawodami."
                                                                             ), QMessageBox::Ok);
    }
    else if(index.row() == 1)
    {
        QMessageBox::information(this, tr("Jak działa generator formy?"), tr(
        "Każdy zawodnik ma swoją \"tendencję formy\", która może być wzrostowa lub spadkowa. Jeśli zawodnik ma np. tendencję wzrostową, jego forma będzie rosnąć.\nTendencja ta ma również swoją siłę. Mocna tendencja spadkowa będzie powodować duże spadki formy i trudno będzie ją odwrócić (W końcu i tak zostanie odwrócona ponieważ czym forma zawodnika jest bardziej skrajna, tym łatwiej odwrócić tendencję w drugą stronę. Np. jeżeli forma jest wysoka, zwiększa się prawdopodobieństwo na tendencję spadkową).\nGenerator formy najpierw zmienia tendencję formy zawodnika w oparciu o aktualną formę i parametry generowania (Parametr zmienności tendencji i wyrównania tendencji) a następnie ustala nową formę zawodnika w oparciu o nową tendencję i parametr zmienności formy (Jeżeli jest mocna tendencja wzrostowa ale mała zmienność formy w generatorze, to wzrost formy nie będzie bardzo zauważalny."), QMessageBox::Ok);
    }
    else if(index.row() == 2)
    {
        QMessageBox::information(this, tr("Do czego służą poszczególne parametry?"), tr(
        "Każdy parametr generatora formy jest wyjaśniony po najchaniu kursorem na jego nazwę."), QMessageBox::Ok);
    }
    else if(index.row() == 3)
    {
        QMessageBox::information(this, tr("Jak ustalić formę na start nowego sezonu?"), tr(
                                 "Aby osiągnąć taki efekt, że zawodnicy na start następnego sezonu mają całkiem inną formę z ułamkiem formy z końcówki poprzedniego sezonum należy wykonać 3 czynności z generatorem formy:\n1. Wygenerować nową tendencję i formę zawodników. Dzięki temu zawodnicy mają odświeżoną formę na nowy sezon. Trzeba ustawić dużą zmienność tendencji i dużą zmienność formy, a także powinno się ustawić mnożnik wyrównania tendencji powyżej 1 (aby forma zawodników wyrównała się do środka na początek sezonu). Zalecane jest też ustawienie minusowego bonusu formy w wysokości około -10, jeśli początek sezonu to \"okres przygotowawczy\" (Dzięki temu na start przygotowań nikt nie będzie miał bardzo wysokiej formy co nie było by do końca realistyczne).\n2. Ustalić każdemu zawodnikowi neutralną tendencję formy (Ustawić maksymalną tendencję na 0). Resztę trzeba ustawić na zero. Robimy to po to, aby każdy zawodnik miał nową niezbyt mocną tendencję ponieważ przez dużą moc tendencji jaka byłaby przez punkt 1, byłyby nierealistycznie duże spadki i wzrosty.\n3. Ustalić każdemu zawodnikowi nową, niezbyt mocną tendencję poprzez ustawienie średniej zmienności tendencji i ZEROWEJ zmienności formy (formy zmieniać nie chcemy). Można również ustawić według uznania bonus tendencji jeżeli chce się aby zawodnikom na start sezonu forma raczej rosła niż spadała (można też zrobić na odwrót).\nPo wykonaniu tych 3 kroków, wszyscy zawodnicy są gotowi do ciekawej walki w następnym sezonie."), QMessageBox::Ok);
    }
}

