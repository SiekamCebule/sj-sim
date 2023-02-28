#include "CharacteristicInputDialog.h"
#include "ui_CharacteristicInputDialog.h"

#include <QStringList>
#include <QStringListModel>

CharacteristicInputDialog::CharacteristicInputDialog(short action, short parentType, const QSet<Characteristic> & existingCharacteristics, Characteristic *characteristicToEdit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CharacteristicInputDialog),
    action(action),
    parentType(parentType),
    existingCharacteristics(existingCharacteristics),
    characteristicToEdit(characteristicToEdit)
{
    ui->setupUi(this);

    if(getAction() == Edit && this->characteristicToEdit == nullptr)
        this->characteristicToEdit = new Characteristic();

    switch(getAction())
    {
    case Add: ui->label_title->setText(tr("Dodaj cechę")); break;
    case Edit: ui->label_title->setText(tr("Edytuj cechę")); break;
    default: ui->label_title->setText(tr("<Błąd>")); break;
    }
    setWindowTitle(ui->label_title->text());
}

CharacteristicInputDialog::~CharacteristicInputDialog()
{
    delete ui;
}

QString CharacteristicInputDialog::getCharacteristicType() const
{
    return dirtyCharacteristicNames.at(ui->comboBox_type->currentIndex());
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

short CharacteristicInputDialog::getParentType() const
{
    return parentType;
}

void CharacteristicInputDialog::setParentType(short newParentType)
{
    parentType = newParentType;
}

QStringList CharacteristicInputDialog::getDirtyCharacteristicNames() const
{
    return dirtyCharacteristicNames;
}

void CharacteristicInputDialog::setDirtyCharacteristicNames(const QStringList &newDirtyCharacteristicNames)
{
    dirtyCharacteristicNames = newDirtyCharacteristicNames;
}

QSet<Characteristic> & CharacteristicInputDialog::getExistingCharacteristics()
{
    return existingCharacteristics;
}

void CharacteristicInputDialog::setExistingCharacteristics(const QSet<Characteristic> &newExistingCharacteristics)
{
    existingCharacteristics = newExistingCharacteristics;
}

void CharacteristicInputDialog::fillComboBox()
{
    ui->comboBox_type->clear();
    QStringListModel * model = new QStringListModel;

    QVector<int> indexes;
    QStringList characteristics = Characteristic::characteristicTypesForSpecificParent(parentType, false);
    for(auto & characteristic : characteristics)
    {
        //qDebug()<<"THE LAST ONE: "<<characteristics.last();
        //qDebug()<<"ITERACJA --> "<<characteristic<<",      "<<existingCharacteristics.contains(Characteristic(characteristic));
        if(existingCharacteristics.contains(Characteristic(characteristic)))
        {
            int index = 0;
            int it;
            for(it = 0; it < characteristics.size(); it++)
            {
                if(characteristics.at(it) == characteristic)
                    index = it;
            }
            indexes.push_back(index);
            characteristics.remove(index);
        }
    }
    for(auto & characteristic : characteristics){
        dirtyCharacteristicNames.push_back(characteristic);
        characteristic = Characteristic::getTypeToDisplay(Characteristic(characteristic));
    }
    model->setStringList(characteristics);

    ui->comboBox_type->setModel(model);
}
