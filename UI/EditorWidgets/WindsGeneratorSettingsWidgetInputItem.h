#ifndef WINDSGENERATORSETTINGSWIDGETINPUTITEM_H
#define WINDSGENERATORSETTINGSWIDGETINPUTITEM_H

#include <QWidget>

#include "CharacteristicsEditor.h"

namespace Ui {
class WindsGeneratorSettingsWidgetInputItem;
}

class WindsGeneratorSettingsWidgetInputItem : public QWidget
{
    Q_OBJECT

public:
    explicit WindsGeneratorSettingsWidgetInputItem(QWidget *parent = nullptr);
    ~WindsGeneratorSettingsWidgetInputItem();

    void removeSubmitButton();

    double getBaseWindStrengthFromInput();
    double getWindStrengthChangeFromInput();
    short getBaseWindDirectionFromInput();
    double getWindDirectionChangeFromInput();

signals:
    void submitted();

private slots:
    void when_submitButtonClicked();

private:
    Ui::WindsGeneratorSettingsWidgetInputItem *ui;

    CharacteristicsEditor * characteristicsEditor;
};

#endif // WINDSGENERATORSETTINGSWIDGETINPUTITEM_H
