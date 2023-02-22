#ifndef JUMPEREDITORWIDGET_H
#define JUMPEREDITORWIDGET_H

#include <QWidget>
#include "../../simulator/Jumper.h"
#include "CharacteristicsEditor.h"

namespace Ui {
class JumperEditorWidget;
}

class JumperEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JumperEditorWidget(Jumper *jumper = nullptr , CharacteristicsEditor * characteristicsEditor = nullptr, QWidget *parent = nullptr);
    ~JumperEditorWidget();

    void fillJumperInfo();

    Jumper getJumperFromWidget();


    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    CharacteristicsEditor *getCharacteristicsEditor() const;
    void setCharacteristicsEditor(CharacteristicsEditor *newCharacteristicsEditor);

signals:
    void submitted();

private slots:
    void on_pushButton_clicked();

private:
    Ui::JumperEditorWidget *ui;
    CharacteristicsEditor * characteristicsEditor;

    Jumper * jumper;
};

#endif // JUMPEREDITORWIDGET_H
