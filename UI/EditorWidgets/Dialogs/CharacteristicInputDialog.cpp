#include "CharacteristicInputDialog.h"
#include "ui_CharacteristicInputDialog.h"

CharacteristicInputDialog::CharacteristicInputDialog(short action, Characteristic *characteristicToEdit, QWidget *parent) :
    action(action),
    characteristicToEdit(characteristicToEdit),
    QDialog(parent),
    ui(new Ui::CharacteristicInputDialog)
{
    ui->setupUi(this);

    if(getAction() == Edit && this->characteristicToEdit == nullptr)
        this->characteristicToEdit = new Characteristic();

    switch(getAction())
    {
    case Add: ui->label_title->setText("Dodaj cechę"); break;
    case Edit: ui->label_title->setText("Edytuj cechę"); break;
    default: ui->label_title->setText("<Błąd>"); break;
    }
}

CharacteristicInputDialog::~CharacteristicInputDialog()
{
    delete ui;
}

QString CharacteristicInputDialog::getCharacteristicType() const
{
    return "cecha";
}

double CharacteristicInputDialog::getCharacteristicLevel() const
{
    return ui->doubleSpinBox_level->value();
}

Characteristic CharacteristicInputDialog::getCharacteristic() const
{
    return Characteristic(getCharacteristicLevel(), getCharacteristicType());
}

short CharacteristicInputDialog::getAction() const
{
    return action;
}

void CharacteristicInputDialog::setAction(short newAction)
{
    action = newAction;
}

Characteristic *CharacteristicInputDialog::getCharacteristicToEdit() const
{
    return characteristicToEdit;
}

void CharacteristicInputDialog::setCharacteristicToEdit(Characteristic *newCharacteristicToEdit)
{
    characteristicToEdit = newCharacteristicToEdit;
}
