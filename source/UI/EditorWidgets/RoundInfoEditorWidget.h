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
    explicit RoundInfoEditorWidget(bool hideGroupsInfo = true, QWidget *parent = nullptr);
    ~RoundInfoEditorWidget();

    void fillRoundInfoInput();
    int getCountFromInput();
    bool getSortStartListFromInput();
    int getSortAfterGroupsFromInput();

    void hideGroupsInfo();
    void showGroupsInfo();

private:
    Ui::RoundInfoEditorWidget *ui;

    RoundInfo * roundInfo;
public:
    RoundInfo *getRoundInfo() const;
    void setRoundInfo(RoundInfo *newRoundInfo);
};

#endif // ROUNDINFOEDITORWIDGET_H
