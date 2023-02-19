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

    qDebug()<<characteristics.size();
}

void CharacteristicsEditor::on_pushButton_add_clicked()
{
    CharacteristicInputDialog dialog(CharacteristicInputDialog::Add);
    dialog.setModal(true);
    if(dialog.exec() == QDialog::Accepted)
    {
        characteristics.insert(dialog.getCharacteristic());
        emit characteristicsChanged();
                qDebug()<<strings.size();
    }
}

void CharacteristicsEditor::updateStrings()
{
    strings.clear();
    for(const auto & characteristic : qAsConst(characteristics))
    {
        strings.push_back(characteristic.getType() + " (Poziom " + QString::number(characteristic.getLevel()) + ")");
    }
    model->setStringList(strings);
}
