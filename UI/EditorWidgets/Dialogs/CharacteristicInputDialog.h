#ifndef CHARACTERISTICINPUTDIALOG_H
#define CHARACTERISTICINPUTDIALOG_H

#include <QDialog>
#include <QString>
#include "../../../simulator/Characteristic.h"

namespace Ui {
class CharacteristicInputDialog;
}

class CharacteristicInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CharacteristicInputDialog(short action = 0, Characteristic * characteristicToEdit = nullptr, QWidget *parent = nullptr);
    ~CharacteristicInputDialog();

    QString getCharacteristicType() const;
    double getCharacteristicLevel() const;
    Characteristic getCharacteristic() const;

    enum Action{
        Add,
        Edit
    };

    short getAction() const;
    void setAction(short newAction);
    Characteristic *getCharacteristicToEdit() const;
    void setCharacteristicToEdit(Characteristic *newCharacteristicToEdit);

private:
    Ui::CharacteristicInputDialog *ui;
    Characteristic * characteristicToEdit;

    short action;
};

#endif // CHARACTERISTICINPUTDIALOG_H
