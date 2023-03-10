#include "CharacteristicsEditor.h"
#include "ui_CharacteristicsEditor.h"

#include "Dialogs/CharacteristicInputDialog.h"

#include <QStringListModel>
#include <QMessageBox>

CharacteristicsEditor::CharacteristicsEditor(short parentType, QWidget *parent) :
    parentType(parentType),
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
    dialog.setParentType(parentType);
    dialog.setModal(true);
    dialog.setExistingCharacteristics(characteristics);
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
    dirtyStrings.clear();
    for(const auto & characteristic : qAsConst(characteristics))
    {
        strings.push_back(Characteristic::getTypeToDisplay(characteristic) + " (Poziom " + QString::number(characteristic.getLevel()) + ")");
        dirtyStrings.push_back(characteristic.getType());
    }
    model->setStringList(strings);
}

short CharacteristicsEditor::getParentType() const
{
    return parentType;
}

void CharacteristicsEditor::setParentType(short newParentType)
{
    parentType = newParentType;
}
// po kazdej probie edycji, usuwa sie jedno z existing count
void CharacteristicsEditor::on_pushButton_edit_clicked()
{
    if(ui->listView_characteristics->selectionModel()->selectedRows().size() > 0){
        int index = ui->listView_characteristics->selectionModel()->selectedRows().first().row();
        CharacteristicInputDialog dialog(CharacteristicInputDialog::Edit);
        dialog.setParentType(parentType);
        dialog.setModal(true);
        dialog.setExistingCharacteristics(characteristics);
        dialog.getExistingCharacteristics().remove(Characteristic(dirtyStrings.at(index)));
        dialog.fillComboBox();
        if(dialog.exec() == QDialog::Accepted)
        {
            if(dialog.getCharacteristicLevel() != 0){
                characteristics.remove(Characteristic(dirtyStrings.at(index)));
                characteristics.insert(dialog.getCharacteristic());
                emit characteristicsChanged();
            }
        }
    }
}

void CharacteristicsEditor::on_pushButton_delete_clicked()
{
    if(ui->listView_characteristics->selectionModel()->selectedRows().size() > 0){
        int index = ui->listView_characteristics->selectionModel()->selectedRows().at(0).row();
        QMessageBox message;
        message.setStyleSheet("background-color: white; color: black;");
        QMessageBox::StandardButton reply;
        reply = message.question(this, "Usuni??cie cechy", "Na pewno chcesz usun???? t?? cech???\n", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, QMessageBox::StandardButton::Yes);
        if(reply == QMessageBox::Yes)
        {
            characteristics.remove(Characteristic(dirtyStrings.at(index)));
            emit characteristicsChanged();
        }
    }
}

