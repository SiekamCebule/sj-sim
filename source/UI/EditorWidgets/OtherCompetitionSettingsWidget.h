#ifndef OTHERCOMPETITIONSETTINGSWIDGET_H
#define OTHERCOMPETITIONSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class OtherCompetitionSettingsWidget;
}

class OtherCompetitionSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OtherCompetitionSettingsWidget(QWidget *parent = nullptr);
    ~OtherCompetitionSettingsWidget();

    double getInrunSnowFromInput();
    double getSuitsWetFromInput();

private:
    Ui::OtherCompetitionSettingsWidget *ui;
};

#endif // OTHERCOMPETITIONSETTINGSWIDGET_H
