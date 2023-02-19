#ifndef CHARACTERISTICINPUTDIALOG_H
#define CHARACTERISTICINPUTDIALOG_H

#include <QDialog>
#include <QString>
#include <QSet>
#include "../../../simulator/Characteristic.h"

namespace Ui {
class CharacteristicInputDialog;
}

class CharacteristicInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CharacteristicInputDialog(short action = 0, short parentType = 0, QSet<Characteristic> * existingCharacteristics = nullptr, Characteristic * characteristicToEdit = nullptr, QWidget *parent = nullptr);
    ~CharacteristicInputDialog();

    QString getCharacteristicType() const;
    double getCharacteristicLevel() const;
    Characteristic getCharacteristic() const;

    enum Action{
        Add,
        Edit
    };

    void fillComboBox();

    short getAction() const;
    void setAction(short newAction);
    Characteristic *getCharacteristicToEdit() const;
    void setCharacteristicToEdit(Characteristic *newCharacteristicToEdit);
    short getParentType() const;
    void setParentType(short newParentType);
    QSet<Characteristic> *getExistingCharacteristics() const;
    void setExistingCharacteristics(QSet<Characteristic> *newExistingCharacteristics);
    QStringList getDirtyCharacteristicNames() const;
    void setDirtyCharacteristicNames(const QStringList &newDirtyCharacteristicNames);

private:
    Ui::CharacteristicInputDialog *ui;
    Characteristic * characteristicToEdit;
    QSet<Characteristic> * existingCharacteristics;

    QStringList dirtyCharacteristicNames;

    short action;
    short parentType;
};

#endif // CHARACTERISTICINPUTDIALOG_H
