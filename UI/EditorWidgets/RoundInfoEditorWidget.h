#ifndef ROUNDINFOEDITORWIDGET_H
#define ROUNDINFOEDITORWIDGET_H

#include <QWidget>
#include "../../competitions/RoundInfo.h"

namespace Ui {
class RoundInfoEditorWidget;
}

class RoundInfoEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoundInfoEditorWidget(QWidget *parent = nullptr);
    ~RoundInfoEditorWidget();

    void fillRoundInfoInput();
    int getCountFromInput();

    RoundInfo *getRoundInfo() const;
    void setRoundInfo(RoundInfo *newRoundInfo);

private:
    Ui::RoundInfoEditorWidget *ui;

    RoundInfo * roundInfo;
};

#endif // ROUNDINFOEDITORWIDGET_H
