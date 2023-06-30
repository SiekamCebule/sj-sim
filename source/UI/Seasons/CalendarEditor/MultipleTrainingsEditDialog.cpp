#include "MultipleTrainingsEditDialog.h"
#include "ui_MultipleTrainingsEditDialog.h"
#include <QComboBox>
#include <QLabel>

MultipleTrainingsEditDialog::MultipleTrainingsEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultipleTrainingsEditDialog)
{
    ui->setupUi(this);
}

MultipleTrainingsEditDialog::~MultipleTrainingsEditDialog()
{
    delete ui;
}

int MultipleTrainingsEditDialog::getTrainingsCount()
{
    return ui->spinBox_trainingsCount->value();
}

CompetitionRules MultipleTrainingsEditDialog::getTrainingsRules()
{
    return trainingsRules;
}

void MultipleTrainingsEditDialog::on_pushButton_trainingsRules_clicked()
{
    QDialog * dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Window);
    dialog->setWindowTitle(tr("Zasady treningów"));
    dialog->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    dialog->setLayout(new QVBoxLayout(this));
    dialog->setStyleSheet("background-color: white; color: black;");


    CompetitionRulesEditorWidget * editor = new CompetitionRulesEditorWidget(this);
    editor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    editor->resetInputs();

    QWidget * widget = new QWidget(dialog);
    QHBoxLayout * layout = new QHBoxLayout(dialog);
    QLabel * label = new QLabel(dialog);
    label->setText("Zasady: ");
    QFont font("Ubuntu", 10);
    font.setBold(true);
    label->setFont(font);
    QComboBox * comboBox = new QComboBox(dialog);
    comboBox->addItem(tr("BRAK"));
    for(auto & rules : *rulesList){
        comboBox->addItem(rules.getName());
    }
    layout->addWidget(label);
    layout->addWidget(comboBox);
    widget->setLayout(layout);
    connect(comboBox, &QComboBox::currentIndexChanged, this, [this, editor, comboBox](){
        if(comboBox->currentIndex() > 0){
            CompetitionRules * rules = const_cast<CompetitionRules *>(&rulesList->at(comboBox->currentIndex() - 1));
            editor->setCompetitionRules(rules);
            editor->fillCompetitionRulesInputs();
        }
        else
            editor->resetInputs();
    });

    dialog->layout()->addWidget(widget);
    dialog->layout()->addWidget(editor);
    connect(editor, &CompetitionRulesEditorWidget::submitted, dialog, &QDialog::accept);
    if(dialog->exec() == QDialog::Accepted){
        trainingsRules = editor->getCompetitionRulesFromWidgetInputs();
    }
    delete label;
    delete comboBox;
    delete layout;
}

void MultipleTrainingsEditDialog::on_pushButton_submit_clicked()
{
    emit submitted();
}

QVector<CompetitionRules> *MultipleTrainingsEditDialog::getRulesList() const
{
    return rulesList;
}

void MultipleTrainingsEditDialog::setRulesList(QVector<CompetitionRules> *newRulesList)
{
    rulesList = newRulesList;
}

