#ifndef CHARACTERISTICSEDITOR_H
#define CHARACTERISTICSEDITOR_H

#include <QWidget>
#include <QSet>
#include <QStringList>
#include <QStringListModel>
#include "../../simulator/Characteristic.h"

namespace Ui {
class CharacteristicsEditor;
}

class CharacteristicsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CharacteristicsEditor(QWidget *parent = nullptr);
    ~CharacteristicsEditor();

    QStringList getStrings() const;

    QSet<Characteristic> getCharacteristics() const;
    void setCharacteristics(const QSet<Characteristic> &newCharacteristics);

signals:
    void characteristicsChanged();

private slots:
    void on_pushButton_add_clicked();
    void updateStrings();

private:
    Ui::CharacteristicsEditor *ui;

    QSet<Characteristic> characteristics;
    QStringList strings;
    QStringListModel * model;
};

#endif // CHARACTERISTICSEDITOR_H
