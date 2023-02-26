#ifndef WINDSGENERATORSETTINGSEDITORWIDGET_H
#define WINDSGENERATORSETTINGSEDITORWIDGET_H

#include <QWidget>
#include <QVector>
#include "../../simulator/Wind.h"
#include "../../simulator/wind-generation/WindGenerationSettings.h"
#include "../../simulator/wind-generation/WindsGenerator.h"

namespace Ui {
class WindsGeneratorSettingsEditorWidget;
}

class WindsGeneratorSettingsEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WindsGeneratorSettingsEditorWidget(QWidget *parent = nullptr);
    ~WindsGeneratorSettingsEditorWidget();

    void fillSettingsInputs(double KPoint = (-1));

    QVector<WindGenerationSettings> *getWindGenerationSettings() const;
    void setWindGenerationSettings(QVector<WindGenerationSettings> *newWindGenerationSettings);
    int getSettingsCount() const;
    void setSettingsCount(int newSettingsCount);
    bool getRemovingSubmitButtons() const;
    void setRemovingSubmitButtons(bool newRemovingSubmitButtons);

private:
    Ui::WindsGeneratorSettingsEditorWidget *ui;

    QVector<WindGenerationSettings> * windGenerationSettings;
    int settingsCount;
    bool removingSubmitButtons;
};

#endif // WINDSGENERATORSETTINGSEDITORWIDGET_H
