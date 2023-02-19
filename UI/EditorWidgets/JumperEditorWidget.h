#ifndef JUMPEREDITORWIDGET_H
#define JUMPEREDITORWIDGET_H

#include <QWidget>
#include "../../simulator/Jumper.h"

namespace Ui {
class JumperEditorWidget;
}

class JumperEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JumperEditorWidget(Jumper *jumper = nullptr ,QWidget *parent = nullptr);
    ~JumperEditorWidget();

    void fillJumperInfo();


    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
private:
    Ui::JumperEditorWidget *ui;

    Jumper * jumper;
};

#endif // JUMPEREDITORWIDGET_H
