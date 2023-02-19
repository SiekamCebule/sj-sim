#include "CharacteristicsEditor.h"
#include "ui_CharacteristicsEditor.h"

#include "Dialogs/CharacteristicInputDialog.h"

#include <QStringListModel>

CharacteristicsEditor::CharacteristicsEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacteristicsEditor)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    ui->listView_characteristics->setModel(model);

    connect(this, &CharacteristicsEditor::characteristicsChanged, this, &CharacteristicsEditor::updateStrings);
}

CharacteristicsEditor::~CharacteristicsEditor()
{
    delete ui;
}

QStringList CharacteristicsEditor::getStrings() const
{
    return strings;
}

QSet<Characteristic> CharacteristicsEditor::getCharacteristics() const
{
    return characteristics;
}

void CharacteristicsEditor::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
{
    characteristics = newCharacteristics;
    emit characteristicsChanged();
}

void CharacteristicsEditor::on_pushButton_add_clicked()
{
    CharacteristicInputDialog dialog(CharacteristicInputDialog::Add);
    dialog.setModal(true);
    dialog.setExistingCharacteristics(const_cast<QSet<Characteristic> *>(&characteristics));
    dialog.fillComboBox();
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog.getCharacteristicLevel() != 0){
            characteristics.insert(dialog.getCharacteristic());
            emit characteristicsChanged();
        }
    }
}

void CharacteristicsEditor::updateStrings()
{
    strings.clear();
    for(const auto & characteristic : qAsConst(characteristics))
    {
        strings.push_back(Characteristic::getTypeToDisplay(characteristic) + " (Poziom " + QString::number(characteristic.getLevel()) + ")");
    }
    model->setStringList(strings);
}

void CharacteristicsEditor::on_pushButton_edit_clicked()
{
    int index = ui->listView_characteristics->selectionModel()->selectedRows().at(0).row();
    CharacteristicInputDialog dialog(CharacteristicInputDialog::Edit);
    dialog.setModal(true);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(dialog.getCharacteristicLevel() != 0){

        }
    }
    // co trzeba zrobić?
}

