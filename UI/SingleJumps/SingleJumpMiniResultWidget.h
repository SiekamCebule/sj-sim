#ifndef SINGLEJUMPMINIRESULTWIDGET_H
#define SINGLEJUMPMINIRESULTWIDGET_H

#include <QWidget>
#include "../../simulator/JumpData.h"

class QMouseEvent;

namespace Ui {
class SingleJumpMiniResultWidget;
}

class SingleJumpMiniResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleJumpMiniResultWidget(QWidget *parent = nullptr);
    ~SingleJumpMiniResultWidget();

    void fillUi();

    JumpData *getJumpData() const;
    void setJumpData(JumpData *newJumpData);

    int getIndexInList() const;
    void setIndexInList(int newIndexInList);

signals:
    void pressed();

protected:
    void mousePressEvent(QMouseEvent * event);

private:
    Ui::SingleJumpMiniResultWidget *ui;

    JumpData * jumpData;
    int indexInList;
};

#endif // SINGLEJUMPMINIRESULTWIDGET_H